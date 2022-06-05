#pragma once

#include "connect_token.h"
#include "interfaces/keyboard_interface.h"

namespace NES {
class Controller {
public:
    void init() const;
    void connect_to_port1(KeyboardInterface *keyboard_interface, ConnectToken);
    void connect_to_port2(KeyboardInterface *keyboard_interface, ConnectToken);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);

private:
    KeyboardInterface *m_port1_keyboard_interface = nullptr;
    KeyboardInterface *m_port2_keyboard_interface = nullptr;
    uint8_t m_snapshot[2]{0, 0};
    bool m_active{false};
};
}  // namespace NES