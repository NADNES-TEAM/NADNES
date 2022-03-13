#pragma once

#include <cstdint>

namespace NES {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class ScreenInterface {
public:
    virtual void setPixel(int row, int column, Color color) = 0;
    virtual void refreshScreen() = 0;
};

}  // namespace NES
