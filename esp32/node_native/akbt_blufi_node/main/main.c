#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"

#include "http_server.h"

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

    wifi_start();
    // For whatever reason, this all has to be in app_main()
    http_server_t server;
    http_server_options_t http_options = HTTP_SERVER_OPTIONS_DEFAULT();
    ESP_ERROR_CHECK( http_server_start(&http_options, &server));
    ESP_ERROR_CHECK(http_register_handler(
        server, "/", HTTP_GET, HTTP_HANDLE_RESPONSE, &http_handler, NULL))

    ble_start();
}