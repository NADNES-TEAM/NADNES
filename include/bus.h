#pragma once
#include <cstdint>
#include <array>
#include "cartridge.h"
#include "connect_token.h"
#include "PPU.h"
namespace NES {
struct Bus {
    std::array<uint8_t, 1 << 11> RAM{};
    uint8_t mem_read(uint16_t addr);
    void mem_write(uint16_t addr, uint8_t data);
    // Connected devices
    Ppu *ppu;
    CpuToCartridgeInterface *cartridge;

    void connect(CpuToCartridgeInterface *cartridge_, ConnectToken) noexcept;
    void connect(Ppu *ppu_, ConnectToken) noexcept;
};
}