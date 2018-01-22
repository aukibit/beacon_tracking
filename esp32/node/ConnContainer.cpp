#include <iomanip>
#include <sstream>
#include <string>

#include "ConnContainer.hpp"

std::string int_to_mac(const uint64_t* mem) {
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(2) << std::hex << (int)((uint8_t *)(mem))[0] << ':';
	stream << std::setfill('0') << std::setw(2) << std::hex << (int)((uint8_t *)(mem))[1] << ':';
	stream << std::setfill('0') << std::setw(2) << std::hex << (int)((uint8_t *)(mem))[2] << ':';
	stream << std::setfill('0') << std::setw(2) << std::hex << (int)((uint8_t *)(mem))[3] << ':';
	stream << std::setfill('0') << std::setw(2) << std::hex << (int)((uint8_t *)(mem))[4] << ':';
	stream << std::setfill('0') << std::setw(2) << std::hex << (int)((uint8_t *)(mem))[5];
	return stream.str();
}

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