#include "gateway_config.h"

#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "nvs.h"

#define GATEWAY_CONFIG_NAMESPACE       "gateway_cfg"
#define GATEWAY_CONFIG_KEY             "settings"
#define GATEWAY_CONFIG_VERSION         2U

typedef struct {
    char wifi_ssid[GATEWAY_WIFI_SSID_LEN + 1];
    char wifi_password[GATEWAY_WIFI_PASSWORD_LEN + 1];
    char mqtt_uri[GATEWAY_MQTT_URI_LEN + 1];
    char mqtt_username[GATEWAY_MQTT_USERNAME_LEN + 1];
    char mqtt_password[GATEWAY_MQTT_PASSWORD_LEN + 1];
    char mqtt_topic[GATEWAY_MQTT_TOPIC_LEN + 1];
} gateway_config_v1_t;

typedef struct {
    uint32_t version;
    gateway_config_t config;
} gateway_config_blob_t;

typedef struct {
    uint32_t version;
    gateway_config_v1_t config;
} gateway_config_blob_v1_t;

static const char *TAG = "gateway_config";

static SemaphoreHandle_t s_config_lock;
static gateway_config_t s_config;

static void gateway_config_set_defaults(gateway_config_t *config)
{
    memset(config, 0, sizeof(*config));
    strlcpy(config->mqtt_topic, "iw/beacons", sizeof(config->mqtt_topic));
}

static void gateway_config_migrate_v1(const gateway_config_blob_v1_t *legacy_blob,
                                      gateway_config_t *out_config)
{
    gateway_config_set_defaults(out_config);
    strlcpy(out_config->wifi_ssid, legacy_blob->config.wifi_ssid, sizeof(out_config->wifi_ssid));
    strlcpy(out_config->wifi_password, legacy_blob->config.wifi_password, sizeof(out_config->wifi_password));
    strlcpy(out_config->mqtt_uri, legacy_blob->config.mqtt_uri, sizeof(out_config->mqtt_uri));
    strlcpy(out_config->mqtt_username, legacy_blob->config.mqtt_username, sizeof(out_config->mqtt_username));
    strlcpy(out_config->mqtt_password, legacy_blob->config.mqtt_password, sizeof(out_config->mqtt_password));
    strlcpy(out_config->mqtt_topic, legacy_blob->config.mqtt_topic, sizeof(out_config->mqtt_topic));

    if (out_config->mqtt_topic[0] == '\0') {
        strlcpy(out_config->mqtt_topic, "iw/beacons", sizeof(out_config->mqtt_topic));
    }
}

static esp_err_t gateway_config_write_nvs(const gateway_config_t *config)
{
    gateway_config_blob_t blob = {
        .version = GATEWAY_CONFIG_VERSION,
        .config = *config,
    };
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(GATEWAY_CONFIG_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_blob(nvs_handle, GATEWAY_CONFIG_KEY, &blob, sizeof(blob));
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
    }

    nvs_close(nvs_handle);
    return err;
}

esp_err_t gateway_config_init(void)
{
    if (s_config_lock == NULL) {
        s_config_lock = xSemaphoreCreateMutex();
        if (s_config_lock == NULL) {
            return ESP_ERR_NO_MEM;
        }
    }

    gateway_config_t defaults;
    gateway_config_set_defaults(&defaults);

    gateway_config_blob_t blob = { 0 };
    gateway_config_blob_v1_t legacy_blob = { 0 };
    size_t blob_size = 0;
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(GATEWAY_CONFIG_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_get_blob(nvs_handle, GATEWAY_CONFIG_KEY, NULL, &blob_size);
    if (err == ESP_OK) {
        if (blob_size == sizeof(blob)) {
            err = nvs_get_blob(nvs_handle, GATEWAY_CONFIG_KEY, &blob, &blob_size);
            if (err == ESP_OK && blob.version == GATEWAY_CONFIG_VERSION) {
                xSemaphoreTake(s_config_lock, portMAX_DELAY);
                s_config = blob.config;
                xSemaphoreGive(s_config_lock);
                nvs_close(nvs_handle);
                return ESP_OK;
            }

            ESP_LOGW(TAG, "Invalid config version in NVS, loading defaults");
        } else if (blob_size == sizeof(legacy_blob)) {
            err = nvs_get_blob(nvs_handle, GATEWAY_CONFIG_KEY, &legacy_blob, &blob_size);
            if (err == ESP_OK && legacy_blob.version == 1U) {
                gateway_config_t migrated_config;

                gateway_config_migrate_v1(&legacy_blob, &migrated_config);
                xSemaphoreTake(s_config_lock, portMAX_DELAY);
                s_config = migrated_config;
                xSemaphoreGive(s_config_lock);
                nvs_close(nvs_handle);
                ESP_LOGI(TAG, "Migrated gateway config from version 1 to version %u", GATEWAY_CONFIG_VERSION);
                return gateway_config_write_nvs(&migrated_config);
            }

            ESP_LOGW(TAG, "Invalid legacy config in NVS, loading defaults");
        } else {
            ESP_LOGW(TAG, "Unexpected config blob size in NVS (%u bytes), loading defaults", (unsigned)blob_size);
        }
    } else if (err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGW(TAG, "Invalid config in NVS, loading defaults (%s)", esp_err_to_name(err));
    }

    xSemaphoreTake(s_config_lock, portMAX_DELAY);
    s_config = defaults;
    xSemaphoreGive(s_config_lock);

    nvs_close(nvs_handle);
    return gateway_config_write_nvs(&defaults);
}

void gateway_config_get(gateway_config_t *out_config)
{
    if (out_config == NULL) {
        return;
    }

    xSemaphoreTake(s_config_lock, portMAX_DELAY);
    *out_config = s_config;
    xSemaphoreGive(s_config_lock);
}

esp_err_t gateway_config_set(const gateway_config_t *new_config)
{
    if (new_config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    gateway_config_t local_copy = *new_config;
    esp_err_t err = gateway_config_write_nvs(&local_copy);
    if (err != ESP_OK) {
        return err;
    }

    xSemaphoreTake(s_config_lock, portMAX_DELAY);
    s_config = local_copy;
    xSemaphoreGive(s_config_lock);

    return ESP_OK;
}

bool gateway_config_has_wifi_credentials(const gateway_config_t *config)
{
    return config != NULL && config->wifi_ssid[0] != '\0';
}

bool gateway_config_has_mqtt_broker(const gateway_config_t *config)
{
    return config != NULL && config->mqtt_uri[0] != '\0';
}
