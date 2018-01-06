// C++ includes
#include <string>

// Package includes
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Local includes
#include "ConnContainer.hpp"
#include "helper_functions.hpp"
#include "LocalWifiServer.hpp"

// Global variables
int scan_time = 5; // seconds
BLEScan* pBLEScan;
BLEScanResults found_devices;
std::string ssid = "";
std::string wifi_password = "auk!b1t";

class AdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        ConnContainer new_conn(
            ESP.getEfuseMac(), 
            advertisedDevice.getAddress().toString(), 
            advertisedDevice.getRSSI()
        );
        Serial.printf(new_conn.print_pretty().c_str());
    }
};

void setup() {
    Serial.begin(115200);

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // More power, but faster
}

void loop() {
    Serial.printf("Starting a scan for %d seconds...\n\r", scan_time);
    found_devices = pBLEScan->start(scan_time);
}  