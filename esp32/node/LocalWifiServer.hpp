#ifndef LOCALWIFISERVER_NODE_H
#define LOCALWIFISERVER_NODE_H

#include <WiFi.h>

class LocalWifiServer {
    WiFiServer server = WiFiServer(80);
public:
    LocalWifiServer(std::string ssid, std::string password);
};

#endif