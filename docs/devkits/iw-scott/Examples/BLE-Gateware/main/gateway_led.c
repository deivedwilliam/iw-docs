#include "gateway_led.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"

#define GATEWAY_LED_GPIO              GPIO_NUM_21
#define GATEWAY_LED_RESOLUTION_HZ     10000000
#define GATEWAY_LED_TASK_STACK        4096
#define GATEWAY_LED_TASK_PRIORITY     4
#define GATEWAY_LED_FRAME_MS          35
#define GATEWAY_LED_FADE_STEPS        36
#define GATEWAY_LED_MAX_BRIGHTNESS    192

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t *bytes_encoder;
    rmt_encoder_t *copy_encoder;
    int state;
    rmt_symbol_word_t reset_code;
} gateway_led_encoder_t;

typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} gateway_led_grb_t;

static const char *TAG = "gateway_led";

static const gateway_led_grb_t GATEWAY_LED_COLOR_CYAN = {
    .green = 0xFF,
    .red = 0x00,
    .blue = 0xFF,
};

static const gateway_led_grb_t GATEWAY_LED_COLOR_GREEN = {
    .green = 0xFF,
    .red = 0x00,
    .blue = 0x00,
};

static SemaphoreHandle_t s_led_lock;
static rmt_channel_handle_t s_led_channel;
static rmt_encoder_handle_t s_led_encoder;
static bool s_led_ready;
static bool s_publish_override_active;

RMT_ENCODER_FUNC_ATTR
static size_t gateway_led_encode(rmt_encoder_t *encoder,
                                 rmt_channel_handle_t channel,
                                 const void *primary_data,
                                 size_t data_size,
                                 rmt_encode_state_t *ret_state)
{
    gateway_led_encoder_t *led_encoder = __containerof(encoder, gateway_led_encoder_t, base);
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    rmt_encode_state_t state = RMT_ENCODING_RESET;
    size_t encoded_symbols = 0;

    switch (led_encoder->state) {
    case 0:
        encoded_symbols += led_encoder->bytes_encoder->encode(led_encoder->bytes_encoder,
                                                              channel,
                                                              primary_data,
                                                              data_size,
                                                              &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            led_encoder->state = 1;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out;
        }
        /* fall through */
    case 1:
        encoded_symbols += led_encoder->copy_encoder->encode(led_encoder->copy_encoder,
                                                             channel,
                                                             &led_encoder->reset_code,
                                                             sizeof(led_encoder->reset_code),
                                                             &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            state |= RMT_ENCODING_COMPLETE;
            led_encoder->state = RMT_ENCODING_RESET;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out;
        }
        break;
    default:
        break;
    }

out:
    *ret_state = state;
    return encoded_symbols;
}

static esp_err_t gateway_led_del_encoder(rmt_encoder_t *encoder)
{
    gateway_led_encoder_t *led_encoder = __containerof(encoder, gateway_led_encoder_t, base);

    rmt_del_encoder(led_encoder->bytes_encoder);
    rmt_del_encoder(led_encoder->copy_encoder);
    free(led_encoder);

    return ESP_OK;
}

RMT_ENCODER_FUNC_ATTR
static esp_err_t gateway_led_reset_encoder(rmt_encoder_t *encoder)
{
    gateway_led_encoder_t *led_encoder = __containerof(encoder, gateway_led_encoder_t, base);

    rmt_encoder_reset(led_encoder->bytes_encoder);
    rmt_encoder_reset(led_encoder->copy_encoder);
    led_encoder->state = RMT_ENCODING_RESET;

    return ESP_OK;
}

static esp_err_t gateway_led_new_encoder(rmt_encoder_handle_t *ret_encoder)
{
    gateway_led_encoder_t *led_encoder = rmt_alloc_encoder_mem(sizeof(gateway_led_encoder_t));
    if (led_encoder == NULL) {
        return ESP_ERR_NO_MEM;
    }

    led_encoder->base.encode = gateway_led_encode;
    led_encoder->base.del = gateway_led_del_encoder;
    led_encoder->base.reset = gateway_led_reset_encoder;

    rmt_bytes_encoder_config_t bytes_encoder_config = {
        .bit0 = {
            .level0 = 1,
            .duration0 = 3,
            .level1 = 0,
            .duration1 = 9,
        },
        .bit1 = {
            .level0 = 1,
            .duration0 = 9,
            .level1 = 0,
            .duration1 = 3,
        },
        .flags.msb_first = 1,
    };
    esp_err_t err = rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder);
    if (err != ESP_OK) {
        free(led_encoder);
        return err;
    }

    rmt_copy_encoder_config_t copy_encoder_config = {};
    err = rmt_new_copy_encoder(&copy_encoder_config, &led_encoder->copy_encoder);
    if (err != ESP_OK) {
        rmt_del_encoder(led_encoder->bytes_encoder);
        free(led_encoder);
        return err;
    }

    led_encoder->reset_code = (rmt_symbol_word_t) {
        .level0 = 0,
        .duration0 = 250,
        .level1 = 0,
        .duration1 = 250,
    };

    *ret_encoder = &led_encoder->base;
    return ESP_OK;
}

static uint8_t gateway_led_scale_component(uint8_t component, uint8_t brightness)
{
    return (uint8_t)(((uint16_t)component * brightness) / 255U);
}

static uint8_t gateway_led_brightness_from_step(uint16_t step)
{
    return (uint8_t)((step * GATEWAY_LED_MAX_BRIGHTNESS) / GATEWAY_LED_FADE_STEPS);
}

static esp_err_t gateway_led_transmit_color(gateway_led_grb_t color, uint8_t brightness)
{
    uint8_t payload[3] = {
        gateway_led_scale_component(color.green, brightness),
        gateway_led_scale_component(color.red, brightness),
        gateway_led_scale_component(color.blue, brightness),
    };
    rmt_transmit_config_t transmit_config = {
        .loop_count = 0,
    };

    esp_err_t err = rmt_transmit(s_led_channel, s_led_encoder, payload, sizeof(payload), &transmit_config);
    if (err != ESP_OK) {
        return err;
    }

    return rmt_tx_wait_all_done(s_led_channel, 20);
}

static void gateway_led_task(void *arg)
{
    bool rising = true;
    uint16_t step = 0;

    (void)arg;

    while (true) {
        bool publish_override_active = false;

        xSemaphoreTake(s_led_lock, portMAX_DELAY);
        publish_override_active = s_publish_override_active;
        xSemaphoreGive(s_led_lock);

        gateway_led_grb_t color = publish_override_active ? GATEWAY_LED_COLOR_GREEN : GATEWAY_LED_COLOR_CYAN;
        uint8_t brightness = gateway_led_brightness_from_step(step);
        esp_err_t err = gateway_led_transmit_color(color, brightness);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "Failed to update LED frame: %s", esp_err_to_name(err));
        }

        if (rising) {
            if (step < GATEWAY_LED_FADE_STEPS) {
                ++step;
            } else {
                rising = false;
            }
        } else if (step > 0) {
            --step;
        } else {
            rising = true;

            xSemaphoreTake(s_led_lock, portMAX_DELAY);
            s_publish_override_active = false;
            xSemaphoreGive(s_led_lock);
        }

        vTaskDelay(pdMS_TO_TICKS(GATEWAY_LED_FRAME_MS));
    }
}

esp_err_t gateway_led_init(void)
{
    if (s_led_ready) {
        return ESP_OK;
    }

    if (s_led_lock == NULL) {
        s_led_lock = xSemaphoreCreateMutex();
        if (s_led_lock == NULL) {
            return ESP_ERR_NO_MEM;
        }
    }

    rmt_tx_channel_config_t channel_config = {
        .gpio_num = GATEWAY_LED_GPIO,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = GATEWAY_LED_RESOLUTION_HZ,
        .mem_block_symbols = SOC_RMT_MEM_WORDS_PER_CHANNEL,
        .trans_queue_depth = 4,
        .intr_priority = 0,
    };

    esp_err_t err = rmt_new_tx_channel(&channel_config, &s_led_channel);
    if (err != ESP_OK) {
        return err;
    }

    err = gateway_led_new_encoder(&s_led_encoder);
    if (err != ESP_OK) {
        return err;
    }

    err = rmt_enable(s_led_channel);
    if (err != ESP_OK) {
        return err;
    }

    BaseType_t task_created = xTaskCreate(gateway_led_task,
                                          "gateway_led",
                                          GATEWAY_LED_TASK_STACK,
                                          NULL,
                                          GATEWAY_LED_TASK_PRIORITY,
                                          NULL);
    if (task_created != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    s_led_ready = true;
    ESP_LOGI(TAG, "LED animation active on GPIO %d", GATEWAY_LED_GPIO);
    return ESP_OK;
}

void gateway_led_notify_publish(void)
{
    if (s_led_lock == NULL) {
        return;
    }

    xSemaphoreTake(s_led_lock, portMAX_DELAY);
    s_publish_override_active = true;
    xSemaphoreGive(s_led_lock);
}
