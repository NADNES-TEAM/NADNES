#pragma once

#include "interfaces/keyboard_interface.h"
#include "connect_token.h"

namespace NES {
class Controller {
public:
    explicit Controller(KeyboardInterface *keyboardInterface = nullptr) : m_keyboard_interface(keyboardInterface) {}

    void init() const;

    void connect(KeyboardInterface *keyboard_interface, ConnectToken);

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);

private:
    KeyboardInterface *m_keyboard_interface;
    uint8_t m_snapshot[2]{0, 0};
    bool m_active{false};
};
}  // namespace NES