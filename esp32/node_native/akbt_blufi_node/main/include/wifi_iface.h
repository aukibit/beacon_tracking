#ifndef AKBT_BLUFI_NODE_WIFI_IFACE
#define AKBT_BLUFI_NODE_WIFI_IFACE

#include "http_server.h"

#define WIFI_SSID "CentralPerk"
#define WIFI_PASS "Sebastian18"

#define DEVICE_IP "192.168.1.70"
#define DEVICE_GW "192.168.1.1"
#define DEVICE_NETMASK "255.255.255.0"
#define DNS_SERVER "8.8.8.8"

void wifi_start(void);
void http_handler (http_context_t http_ctx, void* ctx);

#endif