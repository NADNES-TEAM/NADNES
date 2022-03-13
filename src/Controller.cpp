#include "Controller.h"
#include "ControllerExceptions.h"

namespace NES {

uint8_t Controller::CpuRead(uint16_t addr) {
    uint8_t index = addr - 0x4016;
    uint8_t ans = m_snapshot[index] & 1;
    m_snapshot[index] >>= 1;
    return ans;
}

void Controller::CpuWrite(uint16_t addr, uint8_t value) {
    if (addr != 0x4016) {
        throw InvalidControllerWrite(addr, value);
    }
    if (bool(value & 1) == m_active) {  // nothing to do
        return;
    }
    m_active = (value & 1);
    if (!m_active) {
        m_snapshot[0] = m_keyboardInterface->getPressedKeys();
        m_snapshot[1] = 0;  // Something
    }
}

void Controller::connect(KeyboardInterface *keyboardInterface, ConnectToken) {
    m_keyboardInterface = keyboardInterface;
}

}  // namespace NES
