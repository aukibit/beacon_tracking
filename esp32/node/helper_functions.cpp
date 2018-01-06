#include <sstream>
#include <iomanip>

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
