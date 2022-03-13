#include "ControllerExceptions.h"
//#include <iostream>
#include <sstream>

namespace NES {

std::string get_hex(std::uint16_t addr, bool dollar = true) {
    std::stringstream ss;
    if (dollar)
        ss << "$";
    ss << std::hex << addr;
    return ss.str();
}

InvalidControllerWrite::InvalidControllerWrite(uint16_t addr, uint8_t value)
    : std::runtime_error(std::string("Invalid Controller write: addr=") + get_hex(addr) +
                         ", value=" + get_hex(value, false)) {}

}  // namespace NES
