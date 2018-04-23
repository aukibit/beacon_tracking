#include "string.h"
#include "time.h"

#include "esp_bt.h"
#include "esp_heap_caps.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_ibeacon_api.h"

#include "ble_iface.h"

#define BCN_HUMAN_READABLE_STR  "MAJOR: %d - MINOR: %d - "\
                                "RSSI: %d - TIMESTAMP: %ld"

static const char* AKBT_TAG = "AKBT_BLE";

static void print_beacon (beacon_info * bcn) {
    ESP_LOGD (  AKBT_TAG, BCN_HUMAN_READABLE_STR,
                bcn->major, bcn->minor, bcn->rssi, bcn->timestamp);
}

static void register_beacon (
    esp_ble_ibeacon_t * new_bcn, int * rssi, time_t timestamp) {

    if (numbeacons > 0) {
        for (int i = 0; i < numbeacons; i++) {
            if ((beacons[i]->major == new_bcn->ibeacon_vendor.major) &&
                (beacons[i]->minor == new_bcn->ibeacon_vendor.minor)) {
                beacons[i]->rssi = *rssi;
                beacons[i]->timestamp = timestamp;
                ESP_LOGD(AKBT_TAG, "Old Beacon");
                print_beacon(beacons[i]);
                return;
            }
        }
    }
    beacon_info * beacon_ptr = malloc(sizeof(beacon_info));
    beacon_info ** new_mem = realloc(beacons, (numbeacons + 1) * sizeof(beacon_info*));

    if ((new_mem == NULL) || (beacon_ptr == NULL)) {
        ESP_LOGE(AKBT_TAG, "OUT OF MEMORY");
        // TODO: 
        // There's not enough space to hold the new beacon. Just find the beacon
        // in beacons with the oldest timestamp overwrite it.
        free(beacon_ptr);
        free(new_mem);
        return;
    }

    beacons = new_mem;
    beacons[numbeacons++] = beacon_ptr;

    beacon_ptr->major = new_bcn->ibeacon_vendor.major;
    beacon_ptr->minor = new_bcn->ibeacon_vendor.minor;
    beacon_ptr->rssi = *rssi;
    beacon_ptr->timestamp = timestamp;

    ESP_LOGD(AKBT_TAG, "New Beacon");
    print_beacon(beacon_ptr);

}

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

static esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = 0x50,
    .scan_window            = 0x30
};

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:{
        break;
    }
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT: {
        uint32_t duration = 0; // 0 means scan permanently (in seconds)
        esp_ble_gap_start_scanning(duration);
        break;
    }
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        //scan start complete event to indicate scan start successfully or failed
        if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(AKBT_TAG, "Scan start failed");
        }
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        //adv start complete event to indicate adv start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(AKBT_TAG, "Adv start failed");
        }
        break;
    case ESP_GAP_BLE_SCAN_RESULT_EVT: {
        esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
        switch (scan_result->scan_rst.search_evt) {
        case ESP_GAP_SEARCH_INQ_RES_EVT:
            /* Search for BLE iBeacon Packet */
            if (esp_ble_is_ibeacon_packet(scan_result->scan_rst.ble_adv, scan_result->scan_rst.adv_data_len)){
                register_beacon((esp_ble_ibeacon_t*)(scan_result->scan_rst.ble_adv), &scan_result->scan_rst.rssi, time(0));
                // beacons = register_beacon(ibeacon_data->ibeacon_vendor.proximity_uuid, &scan_result->scan_rst.rssi, time(0));
            }
            break;
        default:
            break;
        }
        break;
    }

    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
        if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
            ESP_LOGE(AKBT_TAG, "Scan stop failed");
        }
        else {
            ESP_LOGI(AKBT_TAG, "Stop scan successfully");
        }
        break;

    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
            ESP_LOGE(AKBT_TAG, "Adv stop failed");
        }
        else {
            ESP_LOGI(AKBT_TAG, "Stop adv successfully");
        }
        break;

    default:
        break;
    }
}

static void ble_ibeacon_appRegister(void)
{
    esp_err_t status;

    //register the scan callback function to the gap module
    if ((status = esp_ble_gap_register_callback(esp_gap_cb)) != ESP_OK) {
        ESP_LOGE(AKBT_TAG, "gap register error, error code = %x", status);
        return;
    }

}

static void ble_ibeacon_init(void)
{
    esp_bluedroid_init();
    esp_bluedroid_enable();
    ble_ibeacon_appRegister();
}

void ble_init (void) {
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);

    ble_ibeacon_init();
    esp_ble_gap_set_scan_params(&ble_scan_params);

    numbeacons = 0;
    beacons = NULL;
}

void beacon_info_to_string (beacon_info * bcn, char * charbuf, int buflen) {
    snprintf(   charbuf, buflen, BCN_HUMAN_READABLE_STR,
                bcn->major, bcn->minor, bcn->rssi, bcn->timestamp);
}