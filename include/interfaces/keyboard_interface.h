#pragma once

#include <cstdint>

namespace NES {

class KeyboardInterface {
public:
    [[nodiscard]] virtual uint8_t get_pressed_keys() const = 0;
};

}  // namespace NES
