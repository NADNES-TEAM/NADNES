#pragma once

#include <cstdint>

namespace NES {

class KeyboardInterface {
public:
    [[nodiscard]] virtual uint8_t getPressedKeys() const = 0;
};

}  // namespace NES
