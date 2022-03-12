#include "KeyboardProcessor.h"
#include <iostream>

namespace NES {

const std::map<Qt::Key, uint8_t> KeyBoardProcessor::bitByKey{{Qt::Key::Key_Z, 0},
                                                             {Qt::Key::Key_X, 1},
                                                             {Qt::Key::Key_Space, 2},
                                                             {Qt::Key::Key_Enter, 3},
                                                             {Qt::Key::Key_Up, 4},
                                                             {Qt::Key::Key_Down, 5},
                                                             {Qt::Key::Key_Left, 6},
                                                             {Qt::Key::Key_Right, 7}};

KeyBoardProcessor::KeyBoardProcessor(QWidget *parent) : QWidget(parent) {}

void KeyBoardProcessor::keyPressEvent(QKeyEvent *event) {
    std::cerr << event->text().toStdString() << std::endl;
    auto it = bitByKey.lower_bound(static_cast<const Qt::Key>(event->key()));
    if (it != bitByKey.end() && it->first == event->key()) {
        std::cout << "WTF????" << std::endl;
        m_state |= (1u << it->second);
    }
}

void KeyBoardProcessor::keyReleaseEvent(QKeyEvent *event) {
    auto it = bitByKey.lower_bound(static_cast<const Qt::Key>(event->key()));
    if (it != bitByKey.end() && it->first == event->key()) {
        m_state &= ~(1u << it->second);
    }
}
uint8_t KeyBoardProcessor::getSnapshot() const {
    return m_state;
}
}  // namespace NES
