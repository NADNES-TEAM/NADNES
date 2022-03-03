#include "screen.h"
#include "screen_options.h"

namespace NES {

void Screen::set_pixel(int row, int column, Color color) {
    if (0 <= row && row < 240 && column >= 0 && column < 256) {
        image.setPixelColor(static_cast<int>(column),
                            static_cast<int>(row),
                            QColor(color.r, color.g, color.b));
    }
}

void Screen::refresh_screen() {
    QSize qSize(WIDTH * WINDOW_SCALE, HEIGHT * WINDOW_SCALE);
    // TODO
    label.setPixmap(QPixmap::fromImage(image.scaled(qSize, Qt::AspectRatioMode::KeepAspectRatio)));
    label.show();
}

Screen::Screen() : image{WIDTH, HEIGHT, QImage::Format_RGB32} {
    image.fill(Qt::GlobalColor::gray);
}

}  // namespace NES
