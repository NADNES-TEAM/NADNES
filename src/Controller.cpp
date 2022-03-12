#include "Controller.h"
#include "ControllerExceptions.h"

namespace NES {

uint8_t Controller::CpuRead(uint16_t addr) {
    uint8_t index = addr - 0x4016;
    if (m_readBit[index] >= 8) {
        return 1;
    } else {
        uint8_t ans = (m_snapshot[index] >> m_readBit[index]) & 1;
        m_readBit[index] = (m_readBit[index] >= 8 ? 8 : m_readBit[index]++);
        return ans;
    }
}

void Controller::CpuWrite(uint16_t addr, uint8_t value) {
    if (addr != 0x4016 || (value & 1) != 1) {
        throw InvalidControllerWrite(addr, value);
    }
    m_snapshot[0] = keyBoardProcessor0.getSnapshot();
    m_snapshot[1] = 0;  // Something
    m_readBit[0] = m_readBit[1] = 0;
}

}  // namespace NES