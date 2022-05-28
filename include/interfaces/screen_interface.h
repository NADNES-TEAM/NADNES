#pragma once

#include <cstdint>
#include <tuple>

namespace NES {

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    bool operator<(const Color &other) const {
        return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
    }
};

class ScreenInterface {
public:
    virtual void set_pixel(int row, int column, Color color) = 0;
    virtual void refresh_screen() = 0;
};

}  // namespace NES
