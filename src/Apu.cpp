#include "Apu.h"
#include "ApuExceptions.h"

namespace NADNES::APU {

void Apu::cpuWrite(uint16_t addr, uint8_t value) {
    bool ok_addr =
        (0x4000 <= addr && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017;

    if (!ok_addr)
        throw NADNESS::APU::BadApuWrite(addr);

    reinterpret_cast<uint8_t *>(this)[addr - 0x4000] = value;

    // TODO: init channels
}

uint8_t Apu::cpuRead(uint16_t addr) {
    if (addr != 0x4015)
        throw NADNESS::APU::BadApuRead(addr);

    return reinterpret_cast<const uint8_t *>(this)[0x4015];
}

}  // namespace NADNES::APU
