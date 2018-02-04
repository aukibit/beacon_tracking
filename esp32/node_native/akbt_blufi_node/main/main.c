#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "nvs_flash.h"

#include "ble_iface.h"
#include "wifi_iface.h"

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ble_init();

    wifi_scan();

}