#ifndef AKBT_BLUFI_NODE_WIFI_IFACE
#define AKBT_BLUFI_NODE_WIFI_IFACE

#include "http_server.h"

#define WIFI_SSID "NETGEAR44"
#define WIFI_PASS "huskytomato221"

void wifi_start(void);
void http_handler (http_context_t http_ctx, void* ctx);

#endif