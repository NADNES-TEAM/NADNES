#pragma once

#include <cstdint>

class KeyboardInterface {
public:
    [[nodiscard]] virtual uint8_t getPressedKeys() const = 0;
};
