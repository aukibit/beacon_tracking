#include <sstream>
#include <string>

#include "ConnContainer.hpp"
#include "helper_functions.hpp"

ConnContainer::ConnContainer(uint64_t chipid, std::string addr, int rssi) :
    esp32_id(chipid), beacon_addr(addr), rssi(rssi) {}

std::string ConnContainer::print_pretty(void) {
    std::string out = "";
    out += "----------\n\rESP32 ID: " + int_to_mac(&this->esp32_id) + "\n\r";
    out += "Beacon ID: " + this->beacon_addr + "\n\r";
    std::stringstream ss;
    ss << this->rssi;
    std::string rssi_str = ss.str();
    out += "RSSI: " + rssi_str;
    out += "\n\r";
    return out;
}