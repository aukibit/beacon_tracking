#include <string.h>

#include "apps/sntp/sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "ble_iface.h"
#include "wifi_iface.h"

static const char* AKBT_TAG = "AKBT_WIFI";
const int CONNECTED_BIT = BIT0;
EventGroupHandle_t wifi_event_group;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_ERROR_CHECK(esp_wifi_connect());
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

void wifi_start(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    ESP_LOGI(AKBT_TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    time(&now);
    localtime_r(&now, &timeinfo);
    while(timeinfo.tm_year < (2016 - 1900)) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}

void http_handler (http_context_t http_ctx, void* ctx) {

    if (!heap_caps_check_integrity_all(true)) {
        ESP_LOGE(AKBT_TAG, "Heap corrupt at the start of http_handler func");
    }

    int bcn_max_chars = 70; // estimate
    int mac_addr_length = 24;
    int res_size = mac_addr_length + bcn_max_chars * numbeacons;
    char res [res_size];
    memset (res, 0, res_size); 

    if (!heap_caps_check_integrity_all(true)) {
        ESP_LOGE(AKBT_TAG, "Heap corrupt after allocating res space");
    }

    uint8_t mac_ptr[6];
    char mac_addr[mac_addr_length]; // "MAC: xx:xx:xx:xx:xx:xx"
    ESP_ERROR_CHECK(esp_efuse_mac_get_default(mac_ptr));

    if (mac_ptr == NULL) {
        ESP_LOGE(AKBT_TAG, "Failed to get MAC address pointer.");
    }
    
    snprintf(mac_addr, mac_addr_length, "MAC: %x:%x:%x:%x:%x:%x", 
            mac_ptr[0], mac_ptr[1], mac_ptr[2],
            mac_ptr[3], mac_ptr[4], mac_ptr[5]);
    strcat(res, mac_addr);
    strcat(res, "<br />");

    if (!heap_caps_check_integrity_all(true)) {
        ESP_LOGE(AKBT_TAG, "Heap corrupt after doing mac addr operations");
    }

    char bcn[bcn_max_chars];
    for (int i = 0; i < numbeacons; i++) {
        memset(bcn, 0, bcn_max_chars);
        beacon_info_to_string(beacons[i], bcn, bcn_max_chars);
        strcat(res, bcn);
        strcat(res, "<br />");
    }    

    if (!heap_caps_check_integrity_all(true)) {
        ESP_LOGE(AKBT_TAG, "Heap corrupt after creating a beacon string");
    }

    http_buffer_t res_buffer = {
        .data = res,
        .size = 0, // null-terminated string
        .data_is_persistent = true // data is in constant RAM;
    };

    http_response_begin(http_ctx, 200, "text/html", HTTP_RESPONSE_SIZE_UNKNOWN);
    http_response_write(http_ctx, &res_buffer);
    http_response_end(http_ctx);

    if (!heap_caps_check_integrity_all(true)) {
        ESP_LOGE(AKBT_TAG, "Heap corrupt at end of http_request");
    }


}