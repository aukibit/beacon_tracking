#ifndef AKBT_BLUFI_NODE_BLE_IFACE
#define AKBT_BLUFI_NODE_BLE_IFACE

typedef struct beacon_info { 
    uint16_t major;
    uint16_t minor;
    int rssi;
    time_t timestamp;
} beacon_info;

beacon_info ** beacons;
uint8_t numbeacons;

void ble_init (void);
void beacon_info_to_string (beacon_info * bcn, char * str, int strlen);

#endif