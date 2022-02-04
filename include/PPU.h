#ifndef NADNES_PPU_H
#define NADNES_PPU_H

#include <cstdint>

namespace NES {
    struct PPU_registers_interface {
        virtual void write_ctrl_reg(uint8_t data) = 0;
        virtual void write_mask_reg(uint8_t data) = 0;
        virtual uint8_t read_status_reg() = 0;
        virtual void write_scroll_reg(uint8_t data) = 0;
    };

    struct OAM_interface {
        virtual void set_address(uint8_t address) = 0;
        virtual void write_data(uint8_t data) = 0;
    };

    struct VRAM_io_interface {
        virtual void set_address(uint8_t address) = 0;
        virtual void write_data(uint8_t data) = 0;
        virtual uint8_t read_data() = 0;
    };


}

#endif //NADNES_PPU_H
