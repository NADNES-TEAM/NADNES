#include "nes_utils.h"

namespace utils {

std::string to_hex8(uint8_t v) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(v);
    return ss.str();
}

std::string to_hex_addr(uint16_t addr) {
    std::stringstream ss;
    ss << "$" << std::hex << std::setfill('0') << std::setw(4) << addr;
    return ss.str();
}
}
