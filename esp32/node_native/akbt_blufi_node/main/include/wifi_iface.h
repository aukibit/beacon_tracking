#ifndef AKBT_BLUFI_NODE_WIFI_IFACE
#define AKBT_BLUFI_NODE_WIFI_IFACE

#include "http_server.h"

#define WIFI_SSID "NETGEAR44"
#define WIFI_PASS "huskytomato221"

#define DEVICE_IP "192.168.0.14"
#define DEVICE_GW "192.168.0.1"
#define DEVICE_NETMASK "255.255.255.0"

void wifi_start(void);
void http_handler (http_context_t http_ctx, void* ctx);

#endif