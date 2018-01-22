// C++ includes
#include <string>

// Package includes
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>

// Local includes
#include "ConnContainer.hpp"

// BLE global variables
int scan_time = 5; // seconds
BLEScan* pBLEScan;
BLEScanResults found_devices;

// WiFi global variables
const char* ssid = "TelstraADBF11";
const char* password = "fvpj9y84fm";
WiFiServer server(80);
WiFiClient client;
std::string ln;
char c;

// Local functions
void serve_wifi_client(void) {
    client = server.available();
    if (client) {
        ln = "";
        while (client.connected()) {
            if (client.available()) {
                c = client.read();
                if (c == '\n') {
                    if (ln.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.print("Success!<br>");
                        client.println();
                        break;
                    } else {
                        ln = "";
                    }
                } else if (c != '\r') {
                    ln += c;
                }
            }
        }
    }
}

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

// Arduino functions
void setup() {
    Serial.begin(115200);

    // BLE setup
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // More power, but faster

    // WiFi setup
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
    }
    server.begin();
    
}

void loop() {
    Serial.printf("Starting a scan for %d seconds...\n\r", scan_time);
    
    /* Can't do both of these one after the other...? */
    // BLE
    found_devices = pBLEScan->start(scan_time);

    // WiFi   
    serve_wifi_client();
}  
