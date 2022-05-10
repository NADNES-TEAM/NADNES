#pragma once

#include <array>
#include <cstdint>
#include "all_nes_fwd.h"
#include "cartridge.h"
#include "connect_token.h"
#include "nes_devices/controller.h"
#include "nes_devices/ppu.h"

namespace NES {
class Bus {
    std::vector<uint8_t> RAM;
    // Connected devices
    Ppu *ppu = nullptr;
    CpuToCartridgeInterface *cartridge = nullptr;
    Controller *controller = nullptr;

public:
    Bus();
    uint8_t mem_read(uint16_t addr);
    void mem_write(uint16_t addr, uint8_t data);
    void connect(CpuToCartridgeInterface *cartridge_, ConnectToken) noexcept;
    void connect(Ppu *ppu_, ConnectToken) noexcept;
    void connect(Controller *controller, ConnectToken) noexcept;
    uint8_t *get_RAM();
};
}  // namespace NES
