#ifndef GATEWAY_RUNTIME_H
#define GATEWAY_RUNTIME_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

typedef struct {
    char mac[18];
    int rssi;
    uint16_t company_id;
    uint8_t version;
    int16_t temperature_x100;
    uint16_t humidity_x100;
    uint8_t battery;
} iw_sensor_reading_t;

typedef struct {
    bool ap_enabled;
    bool sta_connected;
    bool sta_connecting;
    bool mqtt_connected;
    char ap_ssid[33];
    char ap_ip[16];
    char sta_ip[16];
    char mqtt_error[96];
} gateway_status_t;

esp_err_t gateway_runtime_start(void);
esp_err_t gateway_runtime_apply_config(void);
void gateway_runtime_wait_for_network_settle(void);
esp_err_t gateway_runtime_publish_sensor(const iw_sensor_reading_t *reading);
void gateway_runtime_get_status(gateway_status_t *out_status);

#endif
