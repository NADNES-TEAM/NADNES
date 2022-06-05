#pragma once

#include <array>
#include <cstdint>
#include "nes_core/all_nes_fwd.h"
#include "cartridge.h"
#include "connect_token.h"
#include "controller.h"
#include "ppu.h"

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
    void save(std::ostream &file);
    void load(std::istream &file);
};
}  // namespace NES
