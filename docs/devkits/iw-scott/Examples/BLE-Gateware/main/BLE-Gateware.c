
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_coexist.h"
#include "esp_gap_ble_api.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "gateway_config.h"
#include "gateway_led.h"
#include "gateway_runtime.h"
#include "nvs_flash.h"

#define BLE_SCAN_INTERVAL_MS           200
#define BLE_SCAN_WINDOW_MS             60
#define BLE_SCAN_INTERVAL_UNITS        ((BLE_SCAN_INTERVAL_MS * 1000) / 625)
#define BLE_SCAN_WINDOW_UNITS          ((BLE_SCAN_WINDOW_MS * 1000) / 625)
#define SENSOR_QUEUE_DEPTH             16
#define SENSOR_REPUBLISH_MS            30000
#define SENSOR_CACHE_SIZE              16

typedef struct {
    bool valid;
    char mac[18];
    int16_t temperature_x100;
    uint16_t humidity_x100;
    uint8_t battery;
    TickType_t last_tick;
} sensor_cache_entry_t;

static const char *TAG = "BLE-Gateway";
static QueueHandle_t s_sensor_queue;
static sensor_cache_entry_t s_sensor_cache[SENSOR_CACHE_SIZE];

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

static esp_ble_scan_params_t ble_scan_params = {
    .scan_type = BLE_SCAN_TYPE_PASSIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = BLE_SCAN_INTERVAL_UNITS,
    .scan_window = BLE_SCAN_WINDOW_UNITS,
    .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
};

static void iw_format_mac(const uint8_t mac[6], char *out_mac, size_t out_mac_len)
{
    snprintf(out_mac, out_mac_len, "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

static bool iw_parse_sensor_packet(const uint8_t *adv_data,
                                   uint8_t adv_data_len,
                                   const esp_bd_addr_t mac,
                                   int rssi,
                                   iw_sensor_reading_t *out_reading)
{
    if (adv_data == NULL || out_reading == NULL) {
        return false;
    }

    for (int index = 0; index < adv_data_len;) {
        uint8_t field_len = adv_data[index];

        if (field_len == 0) {
            break;
        }
        if ((index + field_len) >= adv_data_len) {
            break;
        }

        uint8_t type = adv_data[index + 1];

        if (type == ESP_BLE_AD_MANUFACTURER_SPECIFIC_TYPE) {
            const uint8_t *manufacturer_data = &adv_data[index + 2];
            uint8_t manufacturer_len = field_len - 1;

            if (manufacturer_len >= 8) {
                uint16_t company_id = manufacturer_data[0] | (manufacturer_data[1] << 8);

                if (company_id == 0x0059 || company_id == 0xFFFF) {
                    memset(out_reading, 0, sizeof(*out_reading));
                    iw_format_mac(mac, out_reading->mac, sizeof(out_reading->mac));
                    out_reading->rssi = rssi;
                    out_reading->company_id = company_id;
                    out_reading->version = manufacturer_data[2];
                    out_reading->temperature_x100 = (int16_t)(manufacturer_data[3] | (manufacturer_data[4] << 8));
                    out_reading->humidity_x100 = (uint16_t)(manufacturer_data[5] | (manufacturer_data[6] << 8));
                    out_reading->battery = manufacturer_data[7];
                    return true;
                }
            }
        }

        index += field_len + 1;
    }

    return false;
}

static bool iw_should_emit_reading(const iw_sensor_reading_t *reading)
{
    TickType_t now = xTaskGetTickCount();
    int oldest_slot = 0;
    TickType_t oldest_tick = portMAX_DELAY;

    for (int index = 0; index < SENSOR_CACHE_SIZE; ++index) {
        sensor_cache_entry_t *entry = &s_sensor_cache[index];

        if (!entry->valid) {
            strlcpy(entry->mac, reading->mac, sizeof(entry->mac));
            entry->temperature_x100 = reading->temperature_x100;
            entry->humidity_x100 = reading->humidity_x100;
            entry->battery = reading->battery;
            entry->last_tick = now;
            entry->valid = true;
            return true;
        }

        if (entry->last_tick < oldest_tick) {
            oldest_tick = entry->last_tick;
            oldest_slot = index;
        }

        if (strcmp(entry->mac, reading->mac) == 0) {
            bool changed = entry->temperature_x100 != reading->temperature_x100 ||
                           entry->humidity_x100 != reading->humidity_x100 ||
                           entry->battery != reading->battery;
            bool expired = (now - entry->last_tick) >= pdMS_TO_TICKS(SENSOR_REPUBLISH_MS);

            if (!changed && !expired) {
                return false;
            }

            entry->temperature_x100 = reading->temperature_x100;
            entry->humidity_x100 = reading->humidity_x100;
            entry->battery = reading->battery;
            entry->last_tick = now;
            return true;
        }
    }

    strlcpy(s_sensor_cache[oldest_slot].mac, reading->mac, sizeof(s_sensor_cache[oldest_slot].mac));
    s_sensor_cache[oldest_slot].temperature_x100 = reading->temperature_x100;
    s_sensor_cache[oldest_slot].humidity_x100 = reading->humidity_x100;
    s_sensor_cache[oldest_slot].battery = reading->battery;
    s_sensor_cache[oldest_slot].last_tick = now;
    s_sensor_cache[oldest_slot].valid = true;
    return true;
}

static void iw_sensor_task(void *arg)
{
    iw_sensor_reading_t reading;

    while (xQueueReceive(s_sensor_queue, &reading, portMAX_DELAY) == pdTRUE) {
        if (!iw_should_emit_reading(&reading)) {
            continue;
        }

        ESP_LOGI("IW_SENSOR", "Beacon %s | RSSI %d | Temp %.2f C | Hum %.2f %% | Batt %u %%",
                 reading.mac,
                 reading.rssi,
                 reading.temperature_x100 / 100.0f,
                 reading.humidity_x100 / 100.0f,
                 reading.battery);

        gateway_runtime_publish_sensor(&reading);
    }

    (void)arg;
}

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    esp_err_t err;

    switch (event) {
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
        ESP_ERROR_CHECK(esp_ble_gap_start_scanning(0));
        break;
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        if ((err = param->scan_start_cmpl.status) != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Scanning start failed: %s", esp_err_to_name(err));
        } else {
            ESP_LOGI(TAG, "BLE scanning started");
        }
        break;
    case ESP_GAP_BLE_SCAN_RESULT_EVT:
        if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
            iw_sensor_reading_t reading;

            if (iw_parse_sensor_packet(param->scan_rst.ble_adv,
                                       param->scan_rst.adv_data_len,
                                       param->scan_rst.bda,
                                       param->scan_rst.rssi,
                                       &reading)) {
                if (xQueueSend(s_sensor_queue, &reading, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Fila de sensores cheia, descartando leitura");
                }
            }
        }
        break;
    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
        if ((err = param->scan_stop_cmpl.status) != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Scanning stop failed: %s", esp_err_to_name(err));
        } else {
            ESP_LOGI(TAG, "BLE scanning stopped");
        }
        break;
    default:
        break;
    }
}

static void ble_ibeacon_app_register(void)
{
    esp_err_t status = esp_ble_gap_register_callback(esp_gap_cb);
    if (status != ESP_OK) {
        ESP_LOGE(TAG, "gap register error: %s", esp_err_to_name(status));
    }
}

static void ble_ibeacon_init(void)
{
    esp_bluedroid_config_t config = BT_BLUEDROID_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_bluedroid_init_with_cfg(&config));
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    ble_ibeacon_app_register();
}

static void iw_init_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void app_main(void)
{
    iw_init_nvs();
    ESP_ERROR_CHECK(gateway_config_init());
    esp_err_t led_err = gateway_led_init();
    if (led_err != ESP_OK) {
        ESP_LOGW(TAG, "LED init failed: %s", esp_err_to_name(led_err));
    }
    ESP_ERROR_CHECK(gateway_runtime_start());
    ESP_LOGI(TAG, "Aguardando Wi-Fi/AP estabilizar antes de iniciar o BLE");
    gateway_runtime_wait_for_network_settle();
    ESP_LOGI(TAG, "Sincronizacao de rede concluida; inicializando BLE");

    s_sensor_queue = xQueueCreate(SENSOR_QUEUE_DEPTH, sizeof(iw_sensor_reading_t));
    assert(s_sensor_queue != NULL);
    xTaskCreate(iw_sensor_task, "iw_sensor_task", 4096, NULL, 5, NULL);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_config = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_config));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));

    ble_ibeacon_init();
    ESP_ERROR_CHECK(esp_coex_preference_set(ESP_COEX_PREFER_BALANCE));
    ESP_ERROR_CHECK(esp_ble_gap_set_scan_params(&ble_scan_params));

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
