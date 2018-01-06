#include "LocalWifiServer.hpp"

LocalWifiServer::LocalWifiServer(std::string ssid, std::string password) {
    WiFi.begin(ssid.c_str(), password.c_str());
    this->server.begin();
}