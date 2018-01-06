#ifndef CONNCONTAINER_NODE_H
#define CONNCONTAINER_NODE_H

#include <string>
#include <cstdint>

class ConnContainer {
    uint64_t esp32_id;
    std::string beacon_addr;
    int rssi;
public:
    ConnContainer (uint64_t, std::string, int);
    std::string print_pretty(void);
};

#endif