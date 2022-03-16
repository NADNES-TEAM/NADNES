#include "environment/main_window.h"
#include <QMap>
#include "nes_properties.h"
#include <iostream>
#include <bitset>

namespace NES {

QMap<Qt::Key, int> MainWindow::m_index_by_key{{Qt::Key::Key_A, 0},
                                            {Qt::Key::Key_S, 1},
                                            {Qt::Key::Key_Tab, 2},
                                            {Qt::Key::Key_Return, 3},
                                            {Qt::Key::Key_Up, 4},
                                            {Qt::Key::Key_Down, 5},
                                            {Qt::Key::Key_Left, 6},
                                            {Qt::Key::Key_Right, 7}};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    unsigned value = m_index_by_key.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
    m_pressed_keys_bitset |= mask;                // 2 operations but still ok
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    unsigned value = m_index_by_key.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = ~(1u << value);  // ~(1 << 8) == 0xFF
    m_pressed_keys_bitset &= mask;                 // 2 operations but still ok
}

uint8_t MainWindow::get_pressed_keys() const {
    return m_pressed_keys_bitset;
}

void MainWindow::set_pixel(int row, int column, Color color) {
    if (0 <= row && row < SCREEN_HEIGHT && 0 <= column && column < SCREEN_WIDTH) {
        m_screen_image.setPixelColor(column, row, QColor(color.r, color.g, color.b));
    }
}

MainWindow::MainWindow()
    : QMainWindow(),
      m_screen_image(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGB888),
      m_image_label(new QLabel) {
    m_image_label->setPixmap(QPixmap::fromImage(m_screen_image));
    m_image_label->setBackgroundRole(QPalette::Window);
    m_image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_image_label->setScaledContents(true);

    setCentralWidget(m_image_label);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void MainWindow::refresh_screen() {
    QSize qSize(SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5);
    m_image_label->setPixmap(QPixmap::fromImage(m_screen_image.scaled(
        qSize,
        Qt::AspectRatioMode::KeepAspectRatio
        )));
    m_image_label->update();
}

ScreenInterface *MainWindow::get_screen_interface() {
    return dynamic_cast<ScreenInterface *>(this);
}
KeyboardInterface *MainWindow::get_keyboard_interface() {
    return dynamic_cast<KeyboardInterface *>(this);
}

}  // namespace NES
