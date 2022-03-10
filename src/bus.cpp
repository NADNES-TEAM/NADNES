#include "bus.h"

namespace NES {

void Bus::mem_write(uint16_t addr, uint8_t data) {
    if (addr >= 0 && addr < 0x2000) {  // RAM
        RAM[addr % 0x800] = data;
    } else if (addr >= 0x2000 && addr < 0x4000) {  // PPU Ctrl regs
        addr%=8;
        if (addr == 0) {
            ppu->write_ctrl_reg(data);
        } else if (addr == 1) {
            ppu->write_mask_reg(data);
        } else if (addr == 3) {
            ppu->set_OAM_address(data);
        } else if (addr == 4) {
            ppu->OAM_write(data);
        } else if (addr == 5) {
            ppu->write_scroll_reg(data);
        } else if (addr == 6) {
            ppu->set_VRAM_address(data);
        } else if (addr == 7) {
            ppu->VRAM_write(data);
        }
    }
    if (addr >= 0x4000 && addr < 0x4020) {  // Controllers + APU
    }
    if (addr >= 0x4020 && addr < 0x6000){
    }
    if (addr >= 0x6000 && addr < 0x8000) {  // SRAM - just do nothing
        return;
    }
    if (addr >= 0x8000) {  // ROM
        cartridge->CPU_write(addr, data);
    }
}
uint8_t Bus::mem_read(uint16_t addr) {
    if (addr < 0x2000) {  // RAM
        return RAM[addr % 0x800];
    }
    if (addr >= 0x2000 && addr < 0x4000) {  // PPU Ctrl regs
        addr%=8;
        if (addr == 2) {
            return ppu->read_status_reg();
        } else if (addr == 4) {
            return ppu->OAM_read();
        } else if (addr == 7) {
            return ppu->VRAM_read();
        }
    }
    if (addr >= 0x4000 && addr < 0x4020) {  // Controllers + APU
    }
    if (addr >= 0x4020 && addr < 0x6000){
    }
    if (addr >= 0x6000 && addr < 0x8000) {  // SRAM - just do nothing
        return 0;
    }
    if (addr >= 0x8000) {  // ROM
        return cartridge->CPU_read(addr);
    }
}
void Bus::connect(CpuToCartridgeInterface *cartridge_, ConnectToken) noexcept {
    cartridge = cartridge_;
}
void Bus::connect(Ppu *ppu_, ConnectToken) noexcept {
    ppu = ppu_;
}
}