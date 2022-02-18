#ifndef NADNES_SCREEN_H
#define NADNES_SCREEN_H

#include <cstdint>
#include <QApplication>
#include <QLabel>

namespace NADNES {
class ScreenInterface {
    virtual void set_pixel(uint8_t row,
                           uint8_t columns,
                           int8_t colour) = 0;
    virtual void refresh_screen() = 0;
};

class Screen : ScreenInterface {
public:
    explicit Screen();

    // 0 <= row < 240
    // 0 <= columns < 256
    // 0 <= colour < 2^6
    void set_pixel(uint8_t row,
                   uint8_t column,
                   int8_t color) override;

    void refresh_screen() override;

private:
    // TODO: change Format_RGB32 to something else
    QImage image;
    QLabel label;
};

}  // namespace NADNES

#endif  // NADNES_SCREEN_H
