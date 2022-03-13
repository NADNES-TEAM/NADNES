#pragma once

#include <QLabel>
#include <QWidget>
#include <QtGui>
#include "KeyboardInterface.h"
#include "connect_token.h"

namespace NES {
class Controller {
public:
    explicit Controller(KeyboardInterface *keyboardInterface = nullptr)
        : m_keyboardInterface(keyboardInterface) {}

    void connect(KeyboardInterface *keyboardInterface, ConnectToken);

    uint8_t CpuRead(uint16_t addr);
    void CpuWrite(uint16_t addr, uint8_t value);

private:
    KeyboardInterface *m_keyboardInterface;
    uint8_t m_snapshot[2]{0, 0};
    bool m_active{false};
};
}  // namespace NES
