#include "gateway_runtime.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cJSON.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_spiffs.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "gateway_config.h"
#include "gateway_led.h"
#include "mqtt_client.h"

#define GATEWAY_SPIFFS_BASE_PATH       "/spiffs"
#define GATEWAY_AP_PASSWORD            "gateway123"
#define GATEWAY_AP_IP                  "192.168.4.1"
#define GATEWAY_HTTP_MAX_BODY          1024
#define GATEWAY_HTTP_STACK_SIZE        8192
#define GATEWAY_WIFI_MAX_RETRIES       5U
#define GATEWAY_WORKER_STACK_SIZE      6144
#define GATEWAY_EVENT_STA_CONNECTED    BIT0
#define GATEWAY_EVENT_AP_ACTIVE        BIT1
#define GATEWAY_ACTION_DISABLE_AP      BIT0
#define GATEWAY_ACTION_START_MQTT      BIT1
#define GATEWAY_ACTION_STOP_MQTT       BIT2
#define GATEWAY_ACTION_ENABLE_FALLBACK BIT3

static const char *TAG = "gateway_runtime";

static SemaphoreHandle_t s_runtime_lock;
static EventGroupHandle_t s_runtime_events;
static TaskHandle_t s_runtime_worker_task;
static httpd_handle_t s_http_server;
static esp_mqtt_client_handle_t s_mqtt_client;
static esp_netif_t *s_sta_netif;
static esp_netif_t *s_ap_netif;
static esp_event_handler_instance_t s_wifi_event_instance;
static esp_event_handler_instance_t s_ip_event_instance;
static bool s_runtime_started;
static bool s_ap_enabled;
static bool s_sta_connected;
static bool s_sta_connecting;
static bool s_mqtt_connected;
static uint8_t s_sta_retry_count;
static bool s_sta_connect_request_pending;
static bool s_reconfigure_disconnect_pending;
static bool s_sta_retry_paused;
static char s_ap_ssid[33];
static char s_sta_ip[16];
static char s_mqtt_error[96];
static char s_mqtt_uri[192];
static char s_mqtt_client_id[GATEWAY_MQTT_CLIENT_ID_LEN + 1];

static esp_err_t gateway_runtime_start_mqtt(void);
static void gateway_runtime_worker_task(void *arg);
static void gateway_runtime_schedule_action(uint32_t action_bits);

static void gateway_runtime_set_mqtt_error(const char *message)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    strlcpy(s_mqtt_error, message, sizeof(s_mqtt_error));
    xSemaphoreGive(s_runtime_lock);
}

static void gateway_runtime_clear_mqtt_error(void)
{
    gateway_runtime_set_mqtt_error("");
}

static void gateway_runtime_set_ap_enabled(bool enabled)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_ap_enabled = enabled;
    xSemaphoreGive(s_runtime_lock);

    if (!enabled && s_runtime_events != NULL) {
        xEventGroupClearBits(s_runtime_events, GATEWAY_EVENT_AP_ACTIVE);
    }
}

static bool gateway_runtime_is_ap_enabled(void)
{
    bool enabled;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    enabled = s_ap_enabled;
    xSemaphoreGive(s_runtime_lock);

    return enabled;
}

static void gateway_runtime_reset_sta_retry_count(void)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_sta_retry_count = 0;
    xSemaphoreGive(s_runtime_lock);
}

static uint8_t gateway_runtime_increment_sta_retry_count(void)
{
    uint8_t retry_count;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    if (s_sta_retry_count < UINT8_MAX) {
        ++s_sta_retry_count;
    }
    retry_count = s_sta_retry_count;
    xSemaphoreGive(s_runtime_lock);

    return retry_count;
}

static void gateway_runtime_set_sta_connect_request_pending(bool pending)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_sta_connect_request_pending = pending;
    xSemaphoreGive(s_runtime_lock);
}

static bool gateway_runtime_consume_sta_connect_request_pending(void)
{
    bool pending;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    pending = s_sta_connect_request_pending;
    s_sta_connect_request_pending = false;
    xSemaphoreGive(s_runtime_lock);

    return pending;
}

static void gateway_runtime_mark_reconfigure_disconnect_pending(void)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_reconfigure_disconnect_pending = true;
    xSemaphoreGive(s_runtime_lock);
}

static void gateway_runtime_clear_reconfigure_disconnect_pending(void)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_reconfigure_disconnect_pending = false;
    xSemaphoreGive(s_runtime_lock);
}

static bool gateway_runtime_consume_reconfigure_disconnect_pending(void)
{
    bool pending;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    pending = s_reconfigure_disconnect_pending;
    s_reconfigure_disconnect_pending = false;
    xSemaphoreGive(s_runtime_lock);

    return pending;
}

static void gateway_runtime_set_sta_state(bool connected, bool connecting, const char *ip)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_sta_connected = connected;
    s_sta_connecting = connecting;
    strlcpy(s_sta_ip, ip != NULL ? ip : "", sizeof(s_sta_ip));
    xSemaphoreGive(s_runtime_lock);

    if (s_runtime_events != NULL) {
        if (connected) {
            xEventGroupSetBits(s_runtime_events, GATEWAY_EVENT_STA_CONNECTED);
        } else {
            xEventGroupClearBits(s_runtime_events, GATEWAY_EVENT_STA_CONNECTED);
        }
    }
}

static void gateway_runtime_set_mqtt_connected(bool connected)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_mqtt_connected = connected;
    xSemaphoreGive(s_runtime_lock);
}

static bool gateway_runtime_is_sta_connected(void)
{
    bool connected;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    connected = s_sta_connected;
    xSemaphoreGive(s_runtime_lock);

    return connected;
}

static bool gateway_runtime_is_sta_connecting(void)
{
    bool connecting;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    connecting = s_sta_connecting;
    xSemaphoreGive(s_runtime_lock);

    return connecting;
}

static void gateway_runtime_set_sta_retry_paused(bool paused)
{
    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_sta_retry_paused = paused;
    xSemaphoreGive(s_runtime_lock);
}

static bool gateway_runtime_is_sta_retry_paused(void)
{
    bool paused;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    paused = s_sta_retry_paused;
    xSemaphoreGive(s_runtime_lock);

    return paused;
}

static void gateway_runtime_schedule_action(uint32_t action_bits)
{
    if (s_runtime_worker_task == NULL) {
        return;
    }

    xTaskNotify(s_runtime_worker_task, action_bits, eSetBits);
}

static void gateway_runtime_build_mqtt_uri(const gateway_config_t *config, char *out_uri, size_t out_uri_len)
{
    if (strstr(config->mqtt_uri, "://") != NULL) {
        strlcpy(out_uri, config->mqtt_uri, out_uri_len);
        return;
    }

    snprintf(out_uri, out_uri_len, "mqtt://%s", config->mqtt_uri);
}

static const char *gateway_runtime_mqtt_connect_return_code_to_string(esp_mqtt_connect_return_code_t code)
{
    switch (code) {
    case MQTT_CONNECTION_ACCEPTED:
        return "accepted";
    case MQTT_CONNECTION_REFUSE_PROTOCOL:
        return "protocol";
    case MQTT_CONNECTION_REFUSE_ID_REJECTED:
        return "id_rejected";
    case MQTT_CONNECTION_REFUSE_SERVER_UNAVAILABLE:
        return "server_unavailable";
    case MQTT_CONNECTION_REFUSE_BAD_USERNAME:
        return "bad_username";
    case MQTT_CONNECTION_REFUSE_NOT_AUTHORIZED:
        return "not_authorized";
    default:
        return "unknown";
    }
}

static const char *gateway_runtime_mqtt_auth_mode(const gateway_config_t *config)
{
    if (config->mqtt_username[0] != '\0' && config->mqtt_password[0] != '\0') {
        return "username+password";
    }
    if (config->mqtt_username[0] != '\0') {
        return "username";
    }
    return "anonymous";
}

static void gateway_runtime_build_default_mqtt_client_id(char *out_client_id, size_t out_client_id_len)
{
    uint8_t mac[6];

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(out_client_id,
             out_client_id_len,
             "IW-Gateway-%02X%02X%02X",
             mac[3],
             mac[4],
             mac[5]);
}

static const char *gateway_runtime_wifi_mode_to_string(wifi_mode_t mode)
{
    switch (mode) {
    case WIFI_MODE_STA:
        return "STA";
    case WIFI_MODE_AP:
        return "AP";
    case WIFI_MODE_APSTA:
        return "APSTA";
    default:
        return "UNKNOWN";
    }
}

static wifi_mode_t gateway_runtime_get_target_wifi_mode(const gateway_config_t *config, bool keep_ap_enabled)
{
    if (!gateway_config_has_wifi_credentials(config)) {
        return WIFI_MODE_AP;
    }

    return keep_ap_enabled ? WIFI_MODE_APSTA : WIFI_MODE_STA;
}

static esp_err_t gateway_runtime_apply_wifi_mode(wifi_mode_t mode, const char *reason)
{
    esp_err_t err = esp_wifi_set_mode(mode);

    if (err == ESP_OK) {
        bool ap_enabled = (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA);
        gateway_runtime_set_ap_enabled(ap_enabled);
        ESP_LOGI(TAG, "Wi-Fi mode -> %s (%s)", gateway_runtime_wifi_mode_to_string(mode), reason);
    }

    return err;
}

static esp_err_t gateway_runtime_set_wifi_mode(const gateway_config_t *config,
                                               bool keep_ap_enabled,
                                               const char *reason)
{
    wifi_mode_t mode = gateway_runtime_get_target_wifi_mode(config, keep_ap_enabled);
    return gateway_runtime_apply_wifi_mode(mode, reason);
}

static esp_err_t gateway_runtime_start_sta_connection(const gateway_config_t *config)
{
    if (!gateway_config_has_wifi_credentials(config)) {
        gateway_runtime_set_sta_connect_request_pending(false);
        gateway_runtime_set_sta_state(false, false, "");
        return ESP_OK;
    }

    if (gateway_runtime_is_sta_retry_paused()) {
        gateway_runtime_set_sta_connect_request_pending(false);
        gateway_runtime_set_sta_state(false, false, "");
        ESP_LOGW(TAG, "Tentativas STA pausadas; aguardando nova configuracao pelo AP");
        return ESP_OK;
    }

    gateway_runtime_set_sta_state(false, true, "");
    gateway_runtime_set_sta_connect_request_pending(false);
    ESP_LOGI(TAG, "Tentando conectar ao SSID '%s'", config->wifi_ssid);

    esp_err_t err = esp_wifi_connect();
    if (err == ESP_OK || err == ESP_ERR_WIFI_CONN) {
        return ESP_OK;
    }
    if (err == ESP_ERR_WIFI_NOT_STARTED) {
        ESP_LOGI(TAG, "STA ainda nao iniciou; conexao ficara pendente");
        gateway_runtime_set_sta_connect_request_pending(true);
        return ESP_OK;
    }

    gateway_runtime_set_sta_state(false, false, "");
    ESP_LOGE(TAG, "esp_wifi_connect falhou: %s", esp_err_to_name(err));
    return err;
}

static esp_err_t gateway_runtime_enable_ap_fallback(const gateway_config_t *config)
{
    (void)config;

    ESP_LOGW(TAG,
             "Ativando AP de fallback em %s apos %u falhas e pausando STA",
             GATEWAY_AP_IP,
             GATEWAY_WIFI_MAX_RETRIES);
    return gateway_runtime_apply_wifi_mode(WIFI_MODE_AP, "fallback after retries");
}

static void gateway_runtime_disable_ap_after_sta_connect(void)
{
    gateway_config_t config;
    gateway_config_get(&config);

    if (!gateway_runtime_is_sta_connected() ||
        !gateway_runtime_is_ap_enabled() ||
        !gateway_config_has_wifi_credentials(&config)) {
        return;
    }

    esp_err_t err = gateway_runtime_set_wifi_mode(&config, false, "sta connected");
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Falha ao desligar AP apos conexao STA: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "AP de provisionamento desligado; painel segue no IP STA");
}

static esp_err_t gateway_runtime_stop_mqtt(void)
{
    esp_mqtt_client_handle_t client = NULL;

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    client = s_mqtt_client;
    s_mqtt_client = NULL;
    s_mqtt_connected = false;
    xSemaphoreGive(s_runtime_lock);

    if (client != NULL) {
        esp_mqtt_client_stop(client);
        esp_mqtt_client_destroy(client);
    }

    return ESP_OK;
}

static void gateway_runtime_worker_task(void *arg)
{
    uint32_t action_bits;
    (void)arg;

    while (true) {
        xTaskNotifyWait(0, UINT32_MAX, &action_bits, portMAX_DELAY);

        if ((action_bits & GATEWAY_ACTION_STOP_MQTT) != 0) {
            gateway_runtime_stop_mqtt();
        }
        if ((action_bits & GATEWAY_ACTION_ENABLE_FALLBACK) != 0) {
            gateway_config_t config;
            gateway_config_get(&config);
            gateway_runtime_enable_ap_fallback(&config);
        }
        if ((action_bits & GATEWAY_ACTION_DISABLE_AP) != 0) {
            gateway_runtime_disable_ap_after_sta_connect();
        }
        if ((action_bits & GATEWAY_ACTION_START_MQTT) != 0) {
            gateway_runtime_start_mqtt();
        }
    }
}

static void gateway_runtime_mqtt_event_handler(void *handler_args,
                                               esp_event_base_t base,
                                               int32_t event_id,
                                               void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        gateway_runtime_set_mqtt_connected(true);
        gateway_runtime_clear_mqtt_error();
        ESP_LOGI(TAG, "MQTT conectado ao broker %s com client_id '%s'", s_mqtt_uri, s_mqtt_client_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        gateway_runtime_set_mqtt_connected(false);
        gateway_runtime_set_mqtt_error("Broker desconectado");
        ESP_LOGW(TAG, "MQTT desconectado do broker %s", s_mqtt_uri[0] != '\0' ? s_mqtt_uri : "(indefinido)");
        break;
    case MQTT_EVENT_ERROR:
        gateway_runtime_set_mqtt_connected(false);
        if (event->error_handle != NULL) {
            char message[96];
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                snprintf(message,
                         sizeof(message),
                         "Broker recusou MQTT (%s/%d)",
                         gateway_runtime_mqtt_connect_return_code_to_string(event->error_handle->connect_return_code),
                         event->error_handle->connect_return_code);
            } else {
                snprintf(message,
                         sizeof(message),
                         "Erro MQTT tipo=%d tls=0x%x errno=%d",
                         event->error_handle->error_type,
                         event->error_handle->esp_tls_last_esp_err,
                         event->error_handle->esp_transport_sock_errno);
            }
            gateway_runtime_set_mqtt_error(message);
            ESP_LOGE(TAG,
                     "Falha MQTT broker=%s tipo=%d refused=%d tls=0x%x stack=0x%x errno=%d (%s)",
                     s_mqtt_uri[0] != '\0' ? s_mqtt_uri : "(indefinido)",
                     event->error_handle->error_type,
                     event->error_handle->connect_return_code,
                     event->error_handle->esp_tls_last_esp_err,
                     event->error_handle->esp_tls_stack_err,
                     event->error_handle->esp_transport_sock_errno,
                     event->error_handle->esp_transport_sock_errno != 0
                         ? strerror(event->error_handle->esp_transport_sock_errno)
                         : "sem errno");
        } else {
            gateway_runtime_set_mqtt_error("Erro MQTT desconhecido");
            ESP_LOGE(TAG, "Falha MQTT: evento de erro sem detalhes");
        }
        break;
    default:
        break;
    }

    (void)handler_args;
    (void)base;
}

static esp_err_t gateway_runtime_start_mqtt(void)
{
    gateway_config_t config;
    gateway_config_get(&config);

    gateway_runtime_stop_mqtt();

    if (!gateway_config_has_mqtt_broker(&config)) {
        ESP_LOGI(TAG, "MQTT desabilitado: broker nao configurado");
        return ESP_OK;
    }

    if (!gateway_runtime_is_sta_connected()) {
        ESP_LOGI(TAG, "MQTT aguardando IP do Wi-Fi para iniciar");
        return ESP_OK;
    }

    gateway_runtime_build_mqtt_uri(&config, s_mqtt_uri, sizeof(s_mqtt_uri));
    if (config.mqtt_client_id[0] != '\0') {
        strlcpy(s_mqtt_client_id, config.mqtt_client_id, sizeof(s_mqtt_client_id));
    } else {
        gateway_runtime_build_default_mqtt_client_id(s_mqtt_client_id, sizeof(s_mqtt_client_id));
    }
    gateway_runtime_clear_mqtt_error();

    esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = s_mqtt_uri,
        .credentials.username = config.mqtt_username[0] != '\0' ? config.mqtt_username : NULL,
        .credentials.client_id = s_mqtt_client_id,
        .credentials.authentication.password = config.mqtt_password[0] != '\0' ? config.mqtt_password : NULL,
    };

    ESP_LOGI(TAG,
             "Iniciando MQTT broker=%s client_id='%s' auth=%s",
             s_mqtt_uri,
             s_mqtt_client_id,
             gateway_runtime_mqtt_auth_mode(&config));

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_config);
    if (client == NULL) {
        gateway_runtime_set_mqtt_error("Falha ao criar cliente MQTT");
        ESP_LOGE(TAG, "Falha ao criar cliente MQTT para broker %s", s_mqtt_uri);
        return ESP_FAIL;
    }

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, gateway_runtime_mqtt_event_handler, NULL);

    esp_err_t err = esp_mqtt_client_start(client);
    if (err != ESP_OK) {
        gateway_runtime_set_mqtt_error("Falha ao iniciar MQTT");
        ESP_LOGE(TAG, "esp_mqtt_client_start falhou: %s", esp_err_to_name(err));
        esp_mqtt_client_destroy(client);
        return err;
    }

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    s_mqtt_client = client;
    xSemaphoreGive(s_runtime_lock);

    return ESP_OK;
}

static esp_err_t gateway_runtime_configure_sta(const gateway_config_t *config)
{
    bool sta_was_active;
    wifi_config_t sta_config = { 0 };

    if (!gateway_config_has_wifi_credentials(config)) {
        gateway_runtime_reset_sta_retry_count();
        gateway_runtime_set_sta_connect_request_pending(false);
        gateway_runtime_clear_reconfigure_disconnect_pending();
        gateway_runtime_set_sta_retry_paused(false);
        gateway_runtime_set_sta_state(false, false, "");
        gateway_runtime_stop_mqtt();
        ESP_LOGI(TAG, "Sem credenciais Wi-Fi salvas; gateway permanece em AP");
        return ESP_OK;
    }

    strlcpy((char *)sta_config.sta.ssid, config->wifi_ssid, sizeof(sta_config.sta.ssid));
    strlcpy((char *)sta_config.sta.password, config->wifi_password, sizeof(sta_config.sta.password));
    sta_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    sta_config.sta.failure_retry_cnt = 1;
    sta_config.sta.threshold.authmode = config->wifi_password[0] == '\0' ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;
    sta_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
    sta_config.sta.pmf_cfg.capable = true;
    sta_config.sta.pmf_cfg.required = false;

    esp_err_t err = esp_wifi_set_config(WIFI_IF_STA, &sta_config);
    if (err != ESP_OK) {
        return err;
    }

    gateway_runtime_reset_sta_retry_count();
    gateway_runtime_clear_reconfigure_disconnect_pending();
    gateway_runtime_set_sta_retry_paused(false);

    sta_was_active = gateway_runtime_is_sta_connected() || gateway_runtime_is_sta_connecting();
    if (!sta_was_active) {
        ESP_LOGI(TAG, "Aplicando Wi-Fi para SSID '%s'", config->wifi_ssid);
        return gateway_runtime_start_sta_connection(config);
    }

    err = esp_wifi_disconnect();
    if (err == ESP_OK) {
        gateway_runtime_mark_reconfigure_disconnect_pending();
        ESP_LOGI(TAG, "Reaplicando Wi-Fi para SSID '%s'", config->wifi_ssid);
        gateway_runtime_set_sta_state(false, true, "");
        return ESP_OK;
    }
    if (err != ESP_OK && err != ESP_ERR_WIFI_NOT_CONNECT) {
        return err;
    }

    return gateway_runtime_start_sta_connection(config);
}

static esp_err_t gateway_runtime_send_json(httpd_req_t *req, cJSON *json)
{
    char *payload = cJSON_PrintUnformatted(json);
    if (payload == NULL) {
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Falha ao montar JSON");
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Cache-Control", "no-store");
    esp_err_t err = httpd_resp_send(req, payload, HTTPD_RESP_USE_STRLEN);

    free(payload);
    cJSON_Delete(json);
    return err;
}

static esp_err_t gateway_runtime_config_get_handler(httpd_req_t *req)
{
    gateway_config_t config;
    gateway_config_get(&config);

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "wifi_ssid", config.wifi_ssid);
    cJSON_AddStringToObject(json, "wifi_password", config.wifi_password);
    cJSON_AddStringToObject(json, "mqtt_uri", config.mqtt_uri);
    cJSON_AddStringToObject(json, "mqtt_username", config.mqtt_username);
    cJSON_AddStringToObject(json, "mqtt_password", config.mqtt_password);
    cJSON_AddStringToObject(json, "mqtt_client_id", config.mqtt_client_id);
    cJSON_AddStringToObject(json, "mqtt_topic", config.mqtt_topic);

    return gateway_runtime_send_json(req, json);
}

static esp_err_t gateway_runtime_status_get_handler(httpd_req_t *req)
{
    gateway_status_t status;
    gateway_runtime_get_status(&status);

    cJSON *json = cJSON_CreateObject();
    cJSON_AddBoolToObject(json, "ap_enabled", status.ap_enabled);
    cJSON_AddStringToObject(json, "ap_ssid", status.ap_ssid);
    cJSON_AddStringToObject(json, "ap_ip", status.ap_ip);
    cJSON_AddBoolToObject(json, "sta_connected", status.sta_connected);
    cJSON_AddBoolToObject(json, "sta_connecting", status.sta_connecting);
    cJSON_AddStringToObject(json, "sta_ip", status.sta_ip);
    cJSON_AddBoolToObject(json, "mqtt_connected", status.mqtt_connected);
    cJSON_AddStringToObject(json, "mqtt_error", status.mqtt_error);

    return gateway_runtime_send_json(req, json);
}

static void gateway_runtime_update_string_field(cJSON *json,
                                                const char *field_name,
                                                char *target,
                                                size_t target_len)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, field_name);
    if (cJSON_IsString(item) && item->valuestring != NULL) {
        strlcpy(target, item->valuestring, target_len);
    }
}

static esp_err_t gateway_runtime_config_post_handler(httpd_req_t *req)
{
    char *body = NULL;

    if (req->content_len <= 0 || req->content_len > GATEWAY_HTTP_MAX_BODY) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Payload invalido");
        return ESP_FAIL;
    }

    body = calloc((size_t)req->content_len + 1, sizeof(char));
    if (body == NULL) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Sem memoria para payload");
        return ESP_ERR_NO_MEM;
    }

    int total_read = 0;

    while (total_read < req->content_len) {
        int read_now = httpd_req_recv(req, body + total_read, req->content_len - total_read);
        if (read_now <= 0) {
            free(body);
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Falha ao ler payload");
            return ESP_FAIL;
        }
        total_read += read_now;
    }
    body[total_read] = '\0';

    cJSON *json = cJSON_Parse(body);
    free(body);
    if (json == NULL) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "JSON invalido");
        return ESP_FAIL;
    }

    gateway_config_t config;
    gateway_config_get(&config);

    gateway_runtime_update_string_field(json, "wifi_ssid", config.wifi_ssid, sizeof(config.wifi_ssid));
    gateway_runtime_update_string_field(json, "wifi_password", config.wifi_password, sizeof(config.wifi_password));
    gateway_runtime_update_string_field(json, "mqtt_uri", config.mqtt_uri, sizeof(config.mqtt_uri));
    gateway_runtime_update_string_field(json, "mqtt_username", config.mqtt_username, sizeof(config.mqtt_username));
    gateway_runtime_update_string_field(json, "mqtt_password", config.mqtt_password, sizeof(config.mqtt_password));
    gateway_runtime_update_string_field(json, "mqtt_client_id", config.mqtt_client_id, sizeof(config.mqtt_client_id));
    gateway_runtime_update_string_field(json, "mqtt_topic", config.mqtt_topic, sizeof(config.mqtt_topic));

    if (config.mqtt_topic[0] == '\0') {
        strlcpy(config.mqtt_topic, "iw/beacons", sizeof(config.mqtt_topic));
    }

    cJSON_Delete(json);

    ESP_LOGI(TAG,
             "Nova configuracao recebida: ssid='%s', client_id='%s', mqtt_uri='%s', topic='%s'",
             config.wifi_ssid,
             config.mqtt_client_id[0] != '\0' ? config.mqtt_client_id : "(auto)",
             config.mqtt_uri,
             config.mqtt_topic);

    esp_err_t err = gateway_config_set(&config);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Falha ao salvar configuracao");
        return err;
    }

    err = gateway_runtime_apply_config();
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Falha ao aplicar configuracao");
        return err;
    }

    cJSON *response = cJSON_CreateObject();
    cJSON_AddBoolToObject(response, "ok", true);
    cJSON_AddStringToObject(response, "message", "Configuracao salva");

    return gateway_runtime_send_json(req, response);
}

static const char *gateway_runtime_content_type_from_path(const char *path)
{
    const char *extension = strrchr(path, '.');

    if (extension == NULL) {
        return "text/plain";
    }
    if (strcmp(extension, ".html") == 0) {
        return "text/html";
    }
    if (strcmp(extension, ".css") == 0) {
        return "text/css";
    }
    if (strcmp(extension, ".js") == 0) {
        return "application/javascript";
    }
    if (strcmp(extension, ".json") == 0) {
        return "application/json";
    }

    return "text/plain";
}

static esp_err_t gateway_runtime_static_get_handler(httpd_req_t *req)
{
    char path[160];
    const char *uri_path = req->uri;
    FILE *file;

    if (strcmp(uri_path, "/") == 0) {
        uri_path = "/index.html";
    }

    if (strstr(uri_path, "..") != NULL) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Caminho invalido");
        return ESP_FAIL;
    }

    if ((strlen(GATEWAY_SPIFFS_BASE_PATH) + strlen(uri_path) + 1) > sizeof(path)) {
        httpd_resp_send_err(req, HTTPD_414_URI_TOO_LONG, "Caminho muito longo");
        return ESP_FAIL;
    }

    strlcpy(path, GATEWAY_SPIFFS_BASE_PATH, sizeof(path));
    strlcat(path, uri_path, sizeof(path));
    file = fopen(path, "rb");
    if (file == NULL) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Arquivo nao encontrado");
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, gateway_runtime_content_type_from_path(path));

    char chunk[512];
    size_t read_bytes;
    while ((read_bytes = fread(chunk, 1, sizeof(chunk), file)) > 0) {
        if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
            fclose(file);
            httpd_resp_sendstr_chunk(req, NULL);
            return ESP_FAIL;
        }
    }

    fclose(file);
    return httpd_resp_send_chunk(req, NULL, 0);
}

static esp_err_t gateway_runtime_start_webserver(void)
{
    httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();
    http_config.stack_size = GATEWAY_HTTP_STACK_SIZE;
    http_config.uri_match_fn = httpd_uri_match_wildcard;
    http_config.max_uri_handlers = 12;

    esp_err_t err = httpd_start(&s_http_server, &http_config);
    if (err != ESP_OK) {
        return err;
    }

    httpd_uri_t config_get = {
        .uri = "/api/config",
        .method = HTTP_GET,
        .handler = gateway_runtime_config_get_handler,
    };
    httpd_uri_t config_post = {
        .uri = "/api/config",
        .method = HTTP_POST,
        .handler = gateway_runtime_config_post_handler,
    };
    httpd_uri_t status_get = {
        .uri = "/api/status",
        .method = HTTP_GET,
        .handler = gateway_runtime_status_get_handler,
    };
    httpd_uri_t static_get = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = gateway_runtime_static_get_handler,
    };

    httpd_register_uri_handler(s_http_server, &config_get);
    httpd_register_uri_handler(s_http_server, &config_post);
    httpd_register_uri_handler(s_http_server, &status_get);
    httpd_register_uri_handler(s_http_server, &static_get);

    return ESP_OK;
}

static esp_err_t gateway_runtime_mount_spiffs(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = GATEWAY_SPIFFS_BASE_PATH,
        .partition_label = "storage",
        .max_files = 8,
        .format_if_mount_failed = true,
    };

    return esp_vfs_spiffs_register(&conf);
}

static void gateway_runtime_wifi_event_handler(void *arg,
                                               esp_event_base_t event_base,
                                               int32_t event_id,
                                               void *event_data)
{
    gateway_config_t config;

    if (event_base != WIFI_EVENT) {
        return;
    }

    switch (event_id) {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG, "Interface STA iniciada");
        if (gateway_runtime_consume_sta_connect_request_pending()) {
            gateway_config_get(&config);
            gateway_runtime_start_sta_connection(&config);
        }
        break;
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "Associado ao roteador Wi-Fi");
        gateway_runtime_set_sta_retry_paused(false);
        gateway_runtime_set_sta_state(false, true, "");
        break;
    case WIFI_EVENT_STA_DISCONNECTED: {
        wifi_event_sta_disconnected_t *disconnected = event_data;
        gateway_runtime_schedule_action(GATEWAY_ACTION_STOP_MQTT);
        gateway_config_get(&config);
        if (!gateway_config_has_wifi_credentials(&config)) {
            gateway_runtime_set_sta_state(false, false, "");
            break;
        }

        if (gateway_runtime_consume_reconfigure_disconnect_pending()) {
            ESP_LOGI(TAG, "Wi-Fi desconectado para reaplicar a configuracao");
            gateway_runtime_start_sta_connection(&config);
            break;
        }

        if (gateway_runtime_is_sta_retry_paused()) {
            gateway_runtime_set_sta_state(false, false, "");
            ESP_LOGW(TAG, "STA permanece pausada; AP de fallback ativo");
            break;
        }

        uint8_t retry_count = gateway_runtime_increment_sta_retry_count();
        gateway_runtime_set_sta_state(false, true, "");
        ESP_LOGW(TAG,
                 "Falha ao conectar no SSID '%s' (reason=%d, tentativa %u)",
                 config.wifi_ssid,
                 disconnected->reason,
                 retry_count);

        if (retry_count == GATEWAY_WIFI_MAX_RETRIES) {
            gateway_runtime_set_sta_retry_paused(true);
            gateway_runtime_schedule_action(GATEWAY_ACTION_ENABLE_FALLBACK);
            gateway_runtime_set_sta_connect_request_pending(false);
            gateway_runtime_set_sta_state(false, false, "");
            ESP_LOGW(TAG, "Limite de tentativas atingido; novas conexoes STA foram pausadas");
            break;
        }

        gateway_runtime_start_sta_connection(&config);
        break;
    }
    case WIFI_EVENT_AP_START:
        ESP_LOGI(TAG, "AP de provisionamento ativo em %s (%s)", GATEWAY_AP_IP, s_ap_ssid);
        if (s_runtime_events != NULL) {
            xEventGroupSetBits(s_runtime_events, GATEWAY_EVENT_AP_ACTIVE);
        }
        break;
    case WIFI_EVENT_AP_STOP:
        ESP_LOGI(TAG, "AP de provisionamento desligado");
        if (s_runtime_events != NULL) {
            xEventGroupClearBits(s_runtime_events, GATEWAY_EVENT_AP_ACTIVE);
        }
        break;
    default:
        break;
    }

    (void)arg;
}

static void gateway_runtime_ip_event_handler(void *arg,
                                             esp_event_base_t event_base,
                                             int32_t event_id,
                                             void *event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        gateway_config_t config;
        ip_event_got_ip_t *got_ip = event_data;
        char ip_buffer[16];

        gateway_config_get(&config);
        snprintf(ip_buffer, sizeof(ip_buffer), IPSTR, IP2STR(&got_ip->ip_info.ip));
        gateway_runtime_reset_sta_retry_count();
        gateway_runtime_set_sta_retry_paused(false);
        gateway_runtime_set_sta_state(true, false, ip_buffer);
        esp_netif_set_default_netif(s_sta_netif);
        ESP_LOGI(TAG, "Wi-Fi conectado no SSID '%s' com IP %s", config.wifi_ssid, ip_buffer);
        ESP_LOGI(TAG, "Painel web disponivel em http://%s", ip_buffer);
        gateway_runtime_schedule_action(GATEWAY_ACTION_DISABLE_AP | GATEWAY_ACTION_START_MQTT);
    }

    (void)arg;
}

static void gateway_runtime_build_ap_ssid(void)
{
    uint8_t mac[6];

    esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
    snprintf(s_ap_ssid, sizeof(s_ap_ssid), "IW-Gateway-%02X%02X%02X", mac[3], mac[4], mac[5]);
}

esp_err_t gateway_runtime_start(void)
{
    gateway_config_t config;

    if (s_runtime_started) {
        return ESP_OK;
    }

    s_runtime_lock = xSemaphoreCreateMutex();
    if (s_runtime_lock == NULL) {
        return ESP_ERR_NO_MEM;
    }
    s_runtime_events = xEventGroupCreate();
    if (s_runtime_events == NULL) {
        return ESP_ERR_NO_MEM;
    }
    if (xTaskCreate(gateway_runtime_worker_task,
                    "gateway_rt",
                    GATEWAY_WORKER_STACK_SIZE,
                    NULL,
                    5,
                    &s_runtime_worker_task) != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    gateway_runtime_build_ap_ssid();
    strlcpy(s_sta_ip, "", sizeof(s_sta_ip));
    strlcpy(s_mqtt_error, "", sizeof(s_mqtt_error));
    gateway_runtime_reset_sta_retry_count();
    gateway_runtime_clear_reconfigure_disconnect_pending();
    gateway_runtime_set_sta_retry_paused(false);

    esp_err_t err = esp_netif_init();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    s_sta_netif = esp_netif_create_default_wifi_sta();
    s_ap_netif = esp_netif_create_default_wifi_ap();
    if (s_sta_netif == NULL || s_ap_netif == NULL) {
        return ESP_FAIL;
    }

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &gateway_runtime_wifi_event_handler,
                                                        NULL,
                                                        &s_wifi_event_instance));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &gateway_runtime_ip_event_handler,
                                                        NULL,
                                                        &s_ip_event_instance));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    wifi_config_t ap_config = {
        .ap = {
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strlcpy((char *)ap_config.ap.ssid, s_ap_ssid, sizeof(ap_config.ap.ssid));
    ap_config.ap.ssid_len = strlen(s_ap_ssid);
    strlcpy((char *)ap_config.ap.password, GATEWAY_AP_PASSWORD, sizeof(ap_config.ap.password));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    gateway_config_get(&config);
    ESP_ERROR_CHECK(gateway_runtime_set_wifi_mode(&config,
                                                  !gateway_config_has_wifi_credentials(&config),
                                                  "initial startup"));

    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    err = gateway_runtime_mount_spiffs();
    if (err != ESP_OK) {
        return err;
    }

    err = gateway_runtime_start_webserver();
    if (err != ESP_OK) {
        return err;
    }

    s_runtime_started = true;
    if (gateway_runtime_is_ap_enabled()) {
        ESP_LOGI(TAG, "AP ativo em %s com SSID %s", GATEWAY_AP_IP, s_ap_ssid);
        ESP_LOGI(TAG, "Senha do AP: %s", GATEWAY_AP_PASSWORD);
    }

    return gateway_runtime_apply_config();
}

esp_err_t gateway_runtime_apply_config(void)
{
    gateway_config_t config;
    esp_err_t err;

    if (!s_runtime_started) {
        return ESP_ERR_INVALID_STATE;
    }

    gateway_config_get(&config);
    bool keep_ap_enabled = gateway_runtime_is_ap_enabled();

    err = gateway_runtime_set_wifi_mode(&config,
                                        keep_ap_enabled || !gateway_config_has_wifi_credentials(&config),
                                        "apply config");
    if (err != ESP_OK) {
        return err;
    }

    err = gateway_runtime_configure_sta(&config);
    if (err != ESP_OK) {
        return err;
    }

    if (!gateway_config_has_mqtt_broker(&config)) {
        gateway_runtime_stop_mqtt();
    } else if (gateway_runtime_is_sta_connected()) {
        err = gateway_runtime_start_mqtt();
    } else {
        ESP_LOGI(TAG, "Broker MQTT configurado; aguardando conexao Wi-Fi para iniciar");
    }

    return err;
}

void gateway_runtime_wait_for_network_settle(void)
{
    if (!s_runtime_started || s_runtime_events == NULL) {
        return;
    }

    xEventGroupWaitBits(s_runtime_events,
                        GATEWAY_EVENT_STA_CONNECTED | GATEWAY_EVENT_AP_ACTIVE,
                        pdFALSE,
                        pdFALSE,
                        portMAX_DELAY);
}

esp_err_t gateway_runtime_publish_sensor(const iw_sensor_reading_t *reading)
{
    gateway_config_t config;
    char payload[256];
    esp_mqtt_client_handle_t client;

    if (reading == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    gateway_config_get(&config);

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    client = s_mqtt_client;
    xSemaphoreGive(s_runtime_lock);

    if (client == NULL || !gateway_config_has_mqtt_broker(&config)) {
        return ESP_ERR_INVALID_STATE;
    }

    snprintf(payload, sizeof(payload),
             "{\"mac\":\"%s\",\"rssi\":%d,\"company_id\":%u,\"version\":%u,"
             "\"temperature_c\":%.2f,\"humidity_pct\":%.2f,\"battery_pct\":%u}",
             reading->mac,
             reading->rssi,
             reading->company_id,
             reading->version,
             reading->temperature_x100 / 100.0f,
             reading->humidity_x100 / 100.0f,
             reading->battery);

    int msg_id = esp_mqtt_client_publish(client, config.mqtt_topic, payload, 0, 1, 0);
    if (msg_id < 0) {
        return ESP_FAIL;
    }

    gateway_led_notify_publish();
    return ESP_OK;
}

void gateway_runtime_get_status(gateway_status_t *out_status)
{
    if (out_status == NULL) {
        return;
    }

    xSemaphoreTake(s_runtime_lock, portMAX_DELAY);
    memset(out_status, 0, sizeof(*out_status));
    out_status->ap_enabled = s_ap_enabled;
    out_status->sta_connected = s_sta_connected;
    out_status->sta_connecting = s_sta_connecting;
    out_status->mqtt_connected = s_mqtt_connected;
    strlcpy(out_status->ap_ssid, s_ap_ssid, sizeof(out_status->ap_ssid));
    strlcpy(out_status->ap_ip, GATEWAY_AP_IP, sizeof(out_status->ap_ip));
    strlcpy(out_status->sta_ip, s_sta_ip, sizeof(out_status->sta_ip));
    strlcpy(out_status->mqtt_error, s_mqtt_error, sizeof(out_status->mqtt_error));
    xSemaphoreGive(s_runtime_lock);
}
