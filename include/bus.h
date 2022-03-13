#pragma once

#include "all_nes_fwd.h"
#include <array>
#include <cstdint>
#include "cartridge.h"
#include "connect_token.h"
#include "PPU.h"

namespace NES {
class Bus {
    std::array<uint8_t, 1 << 11> RAM{};
    // Connected devices
    Ppu *ppu = nullptr;
    CpuToCartridgeInterface *cartridge = nullptr;

public:
    uint8_t mem_read(uint16_t addr);
    void mem_write(uint16_t addr, uint8_t data);
    void connect(CpuToCartridgeInterface *cartridge_, ConnectToken) noexcept;
    void connect(Ppu *ppu_, ConnectToken) noexcept;
};
}  // namespace NES