#include "screen.h"
#include "screen_options.h"

namespace NES {

void Screen::set_pixel(uint8_t row, uint8_t column, int8_t color) {
    int R = (color >> 4) & 0b11;
    int G = (color >> 2) & 0b11;
    int B = color & 0b11;
    // TODO: tune colors
    R *= 64;
    G *= 64;
    B *= 64;
    image.setPixelColor(static_cast<int>(row), static_cast<int>(column),
                        QColor(R, G, B));
}

void Screen::refresh_screen() {
    QSize qSize(WIDTH * WINDOW_SCALE, HEIGHT * WINDOW_SCALE);
    // TODO
    label.setPixmap(QPixmap::fromImage(image.scaled(
        qSize,
        Qt::AspectRatioMode::KeepAspectRatio
        )));
    label.show();
}

Screen::Screen() : image{WIDTH, HEIGHT, QImage::Format_RGB32} {
    image.fill(Qt::GlobalColor::gray);
}

}  // namespace NES
