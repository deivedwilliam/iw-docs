#ifndef GATEWAY_CONFIG_H
#define GATEWAY_CONFIG_H

#include <stdbool.h>

#include "esp_err.h"

#define GATEWAY_WIFI_SSID_LEN          32
#define GATEWAY_WIFI_PASSWORD_LEN      64
#define GATEWAY_MQTT_URI_LEN           128
#define GATEWAY_MQTT_USERNAME_LEN      64
#define GATEWAY_MQTT_PASSWORD_LEN      64
#define GATEWAY_MQTT_CLIENT_ID_LEN     64
#define GATEWAY_MQTT_TOPIC_LEN         96

typedef struct {
    char wifi_ssid[GATEWAY_WIFI_SSID_LEN + 1];
    char wifi_password[GATEWAY_WIFI_PASSWORD_LEN + 1];
    char mqtt_uri[GATEWAY_MQTT_URI_LEN + 1];
    char mqtt_username[GATEWAY_MQTT_USERNAME_LEN + 1];
    char mqtt_password[GATEWAY_MQTT_PASSWORD_LEN + 1];
    char mqtt_client_id[GATEWAY_MQTT_CLIENT_ID_LEN + 1];
    char mqtt_topic[GATEWAY_MQTT_TOPIC_LEN + 1];
} gateway_config_t;

esp_err_t gateway_config_init(void);
void gateway_config_get(gateway_config_t *out_config);
esp_err_t gateway_config_set(const gateway_config_t *new_config);

bool gateway_config_has_wifi_credentials(const gateway_config_t *config);
bool gateway_config_has_mqtt_broker(const gateway_config_t *config);

#endif
