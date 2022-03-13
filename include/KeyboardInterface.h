#pragma once

#include <cstdint>

class KeyboardInterface {
public:
    virtual uint8_t getPressedKeys() const = 0;
};
