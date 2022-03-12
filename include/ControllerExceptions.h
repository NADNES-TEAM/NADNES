#pragma once

#include <stdexcept>

namespace NES {

class InvalidControllerWrite : std::runtime_error {
public:
    InvalidControllerWrite(uint16_t addr, uint8_t value);
};

}  // namespace NES
