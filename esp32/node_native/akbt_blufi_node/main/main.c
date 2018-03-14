#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
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


    // At the moment about one time in ten the thing won't automatically
    // reboot. Need to sort this out or else we're technically looking at
    // a thirty second boot time.
    wifi_scan();
    ble_init();

    // TODO:
    // set_time();
}