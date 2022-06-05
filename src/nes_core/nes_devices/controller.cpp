#include "nes_devices/controller.h"
#include "nes_core/nes_exceptions.h"

namespace NES {

uint8_t Controller::read(uint16_t addr) {
    uint8_t index = addr - 0x4016;
    uint8_t ans = m_snapshot[index] & 1;
    m_snapshot[index] >>= 1;
    return ans;
}

void Controller::write(uint16_t addr, uint8_t value) {
    if (addr != 0x4016) {
        throw ControllerWriteError(addr, value);
    }
    if ((value & 1) == m_active) {  // nothing to do
        return;
    }
    m_active = (value & 1);
    if (!m_active) {
        m_snapshot[0] = m_port1_keyboard_interface->get_pressed_keys();
        m_snapshot[1] = m_port2_keyboard_interface->get_pressed_keys();
    }
}

void Controller::connect_to_port1(KeyboardInterface *keyboard_interface, ConnectToken) {
    m_port1_keyboard_interface = keyboard_interface;
}

void Controller::connect_to_port2(KeyboardInterface *keyboard_interface, ConnectToken) {
    m_port2_keyboard_interface = keyboard_interface;
}

void Controller::init() const {
    if (m_port1_keyboard_interface == nullptr) {
        throw UninitializedController1InterfaceError();
    }
}

}  // namespace NES
