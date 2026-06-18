#ifndef GATEWAY_LED_H
#define GATEWAY_LED_H

#include "esp_err.h"

esp_err_t gateway_led_init(void);
void gateway_led_notify_publish(void);

#endif
