#ifndef NADNES_SCREEN_H
#define NADNES_SCREEN_H

#include <cstdint>
#include <QApplication>
#include <QLabel>

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
public:
    explicit Screen();

    void set_pixel(std::size_t row,
                   std::size_t column,
                   int8_t color) override;

    void refresh_screen() override;

private:
    // TODO: change Format_RGB32 to something else
    QImage image;
    QLabel label;
};

}  // namespace NADNESS

#endif  // NADNES_SCREEN_H
