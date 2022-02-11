#include "screen.h"
#include "screen_options.h"

namespace NADNESS {

void Screen::set_pixel(std::size_t row, std::size_t column, int8_t color) {
    int R = color >> 4;
    int G = (color >> 2) & 0b11;
    int B = color & 0b11;
    // TODO: come up with a better formula
    R *= 64;
    G *= 64;
    B *= 64;
    image.setPixelColor(static_cast<int>(row), static_cast<int>(column),
                        QColor(R, G, B));
}

void Screen::refresh_screen() {
    // TODO: make screen scalable
    QSize qSize(WIDTH * 3, HEIGHT * 3);
    label.setPixmap(QPixmap::fromImage(image.scaled(
        qSize,
        Qt::AspectRatioMode::KeepAspectRatio
        )));
    label.show();
}

// TODO: change Format to something more suitable
Screen::Screen() : image{WIDTH, HEIGHT, QImage::Format_RGB32} {
    image.fill(Qt::GlobalColor::cyan);
}

}  // namespace NADNESS