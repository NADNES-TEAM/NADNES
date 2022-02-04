#ifndef NADNES_SCREEN_H
#define NADNES_SCREEN_H

#include <cstdint>

namespace NADNESS {
class ScreenInterface {
    // 0 <= row < 240
    // 0 <= columns < 256
    // 0 <= colour < 2^6
    virtual void set_pixel(std::size_t row,
                          std::size_t columns,
                          int8_t colour) = 0;
    virtual void refresh_screen() = 0;
};

class Screen : ScreenInterface {
};

}  // namespace NADNESS

#endif  // NADNES_SCREEN_H
