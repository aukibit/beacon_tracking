#ifndef AKBT_BLUFI_NODE_BLE_IFACE
#define AKBT_BLUFI_NODE_BLE_IFACE

typedef struct beacon_info { 
    uint8_t uuid[16];
    int rssi;
    time_t timestamp;
} beacon_info;

beacon_info ** beacons;
uint8_t numbeacons;

void ble_init (void);
size_t ble_stringify(char * strptr);

#endif