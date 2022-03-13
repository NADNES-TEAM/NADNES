#include "QtMainWindow.h"
#include <QMap>
#include "ScreenOptions.h"

namespace NES {

QMap<Qt::Key, int> QtMainWindow::m_indexByKey{{Qt::Key::Key_A, 0},
                                              {Qt::Key::Key_S, 1},
                                              {Qt::Key::Key_Tab, 2},
                                              {Qt::Key::Key_Enter, 3},
                                              {Qt::Key::Key_Up, 4},
                                              {Qt::Key::Key_Down, 5},
                                              {Qt::Key::Key_Left, 6},
                                              {Qt::Key::Key_Right, 7}};

void QtMainWindow::keyPressEvent(QKeyEvent *event) {
    unsigned value = m_indexByKey.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
    m_pressedKeysBitset |= mask;                // 2 operations but still ok
}

void QtMainWindow::keyReleaseEvent(QKeyEvent *event) {
    unsigned value = m_indexByKey.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = ~(1u << value);  // ~(1 << 8) == 0xFF
    m_pressedKeysBitset &= mask;                 // 2 operations but still ok
}

uint8_t QtMainWindow::getPressedKeys() const {
    return m_pressedKeysBitset;
}

void QtMainWindow::setPixel(int row, int column, Color color) {
    m_screenImage.setPixelColor(column, row, QColor(color.r, color.g, color.b));
}
QtMainWindow::QtMainWindow(QWindow *parent)
    : QWindow(parent), ScreenInterface(), KeyboardInterface(), m_screenImage(WIDTH, HEIGHT, QColor::) {}

}  // namespace NES
