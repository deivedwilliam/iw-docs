idf_component_register(
    SRCS
        "BLE-Gateware.c"
        "gateway_config.c"
        "gateway_led.c"
        "gateway_runtime.c"
    PRIV_REQUIRES
        bt
        cjson
        esp_coex
        esp_driver_rmt
        esp_event
        esp_http_server
        esp_netif
        esp_wifi
        nvs_flash
        spiffs
    INCLUDE_DIRS
        "."
)

set(FRONT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../front/build")
if(EXISTS ${FRONT_BUILD_DIR})
    spiffs_create_partition_image(storage ${FRONT_BUILD_DIR} FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "'${FRONT_BUILD_DIR}' doesn't exist. Create the web build first.")
endif()
