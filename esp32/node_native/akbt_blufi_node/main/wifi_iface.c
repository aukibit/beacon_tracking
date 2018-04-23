#include <string.h>

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

#if CONFIG_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_SCAN_METHOD*/

#if CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_SORT_METHOD*/

#if CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_EXAMPLE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_FAST_SCAN_THRESHOLD*/

static const char* AKBT_TAG = "AKBT_WIFI";
const int CONNECTED_BIT = BIT0;
EventGroupHandle_t wifi_event_group;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(AKBT_TAG, "SYSTEM_EVENT_STA_START");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(AKBT_TAG, "SYSTEM_EVENT_STA_GOT_IP");
            ESP_LOGI(AKBT_TAG, "Got IP: %s",
                     ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(AKBT_TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        default:
            break;
    }
    return ESP_OK;
}

void wifi_start(void)
{
    tcpip_adapter_init();

    /* 
    FIXME: For some reason this code (which sets static IP)
    causes the http_handler callback not to get called? 
    And when it does work, it'll randomly throw a heap error after a 
    couple of callbacks...
    */
    /*
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA); // Don't run a DHCP client
    tcpip_adapter_ip_info_t ip_info;
    inet_pton(AF_INET, DEVICE_IP, &ip_info.ip);
    inet_pton(AF_INET, DEVICE_GW, &ip_info.gw);
    inet_pton(AF_INET, DEVICE_NETMASK, &ip_info.netmask);
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
    //*/
    
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_event_group = xEventGroupCreate();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .scan_method = DEFAULT_SCAN_METHOD,
            .sort_method = DEFAULT_SORT_METHOD,
            .threshold.rssi = DEFAULT_RSSI,
            .threshold.authmode = DEFAULT_AUTHMODE,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    xEventGroupWaitBits(
        wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
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

    uint8_t mac_container[6];
    uint8_t * mac_ptr = malloc(sizeof(uint8_t*));
    char mac_addr[mac_addr_length]; // "MAC: xx:xx:xx:xx:xx:xx"
    ESP_ERROR_CHECK(esp_efuse_mac_get_default(mac_ptr));
    memcpy(mac_container, mac_ptr, 6);
    snprintf(mac_addr, mac_addr_length, "MAC: %x:%x:%x:%x:%x:%x", 
            mac_container[0], mac_container[1], mac_container[2],
            mac_container[3], mac_container[4], mac_container[5]);
    strcat(res, mac_addr);
    strcat(res, "<br />");
    free(mac_ptr);

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

    ESP_LOGI(AKBT_TAG, "About to print %s", res);

    http_response_begin(http_ctx, 200, "text/html", HTTP_RESPONSE_SIZE_UNKNOWN);
    http_response_write(http_ctx, &res_buffer);
    http_response_end(http_ctx);

    if (!heap_caps_check_integrity_all(true)) {
        ESP_LOGE(AKBT_TAG, "Heap corrupt at end of http_request");
    }


}