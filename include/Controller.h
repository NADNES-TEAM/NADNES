#pragma once

#include <QLabel>
#include <QWidget>
#include <QtGui>
#include "KeyboardProcessor.h"

namespace NES {
class Controller {
public:
    Controller() {}

    uint8_t CpuRead(uint16_t addr);
    void CpuWrite(uint16_t addr, uint8_t value);

private:
    KeyBoardProcessor keyBoardProcessor0;  // For the first controller
    uint8_t m_snapshot[2]{0, 0};
    uint8_t m_readBit[2]{0, 0};
};
}  // namespace NES