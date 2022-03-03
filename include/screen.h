#pragma once

#include <cstdint>
#include <QApplication>
#include <QLabel>

namespace NES {

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};


struct ScreenInterface {
    virtual void set_pixel(int row, int columns,
                           Color color) = 0;
    virtual void refresh_screen() = 0;
};

class Screen : public ScreenInterface {
public:
    Screen();

    // 0 <= row < 240
    // 0 <= columns < 256
    // 0 <= colour < 2^6
    void set_pixel(int row, int column,
                   Color color) override;

    void refresh_screen() override;

private:
    // TODO: change Format_RGB32 to something else
    QImage image;
    QLabel label;
};

}  // namespace NES

