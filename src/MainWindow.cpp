#include "MainWindow.h"
#include <QMap>
#include "ScreenOptions.h"
#include <iostream>
#include <bitset>

namespace NES {

QMap<Qt::Key, int> MainWindow::m_indexByKey{{Qt::Key::Key_A, 0},
                                            {Qt::Key::Key_S, 1},
                                            {Qt::Key::Key_Tab, 2},
                                            {Qt::Key::Key_Enter, 3},
                                            {Qt::Key::Key_Up, 4},
                                            {Qt::Key::Key_Down, 5},
                                            {Qt::Key::Key_Left, 6},
                                            {Qt::Key::Key_Right, 7}};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    unsigned value = m_indexByKey.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
    uint8_t snapshot = mask;
    std::cerr << int(snapshot) << std::endl;
    m_pressedKeysBitset |= mask;                // 2 operations but still ok
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    unsigned value = m_indexByKey.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = ~(1u << value);  // ~(1 << 8) == 0xFF
    m_pressedKeysBitset &= mask;                 // 2 operations but still ok
}

uint8_t MainWindow::getPressedKeys() const {
    if (m_pressedKeysBitset) {
        std::cerr << "Something works" << std::endl;
        std::cerr << std::bitset<8>(m_pressedKeysBitset) << std::endl;
    }
    return m_pressedKeysBitset;
}

void MainWindow::setPixel(int row, int column, Color color) {
    m_screenImage.setPixelColor(column, row, QColor(color.r, color.g, color.b));
}

MainWindow::MainWindow()
    : QMainWindow(),
      m_screenImage(WIDTH, HEIGHT, QImage::Format_RGB888),
      m_imageLabel(new QLabel) {
    m_imageLabel->setBackgroundRole(QPalette::Window);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imageLabel->setScaledContents(true);

    setCentralWidget(m_imageLabel);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void MainWindow::refreshScreen() {
    update();
}
ScreenInterface *MainWindow::getScreenInterface() {
    return dynamic_cast<ScreenInterface *>(this);
}
KeyboardInterface *MainWindow::getKeyboardInterface() {
    return dynamic_cast<KeyboardInterface *>(this);
}

}  // namespace NES
