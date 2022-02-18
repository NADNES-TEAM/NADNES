#pragma once

#include <cstdint>
#include "screen.h"

namespace NES {
    struct PPURegistersInterface {
        virtual void write_ctrl_reg(uint8_t data) = 0;

        virtual void write_mask_reg(uint8_t data) = 0;

        virtual uint8_t read_status_reg() = 0;

        virtual void write_scroll_reg(uint8_t data) = 0;
    };

    struct OAMInterface {
        virtual void set_OAM_address(uint8_t address) = 0;

        virtual void OAM_write(uint8_t data) = 0;
    };

    struct VRAMIoInterface {
        virtual void set_VRAM_address(uint8_t address) = 0;

        virtual void VRAM_write(uint8_t data) = 0;

        virtual uint8_t VRAM_read_data() = 0;
    };

    struct AddressReg {
        void increase_y_scroll();

        void increase_x_scroll();

        void set_y_scroll_from(const AddressReg &other);

        void set_x_scroll_from(const AddressReg &other);

        union {
            struct {
                uint16_t coarse_x_scroll: 5;
                uint16_t coarse_y_scroll: 5;
                uint16_t nametable_x: 1;
                uint16_t nametable_y: 1;
                uint16_t fine_y_scroll: 4;
            };
            uint16_t reg;
        };
    };

    class PPU : public PPURegistersInterface, public OAMInterface, public VRAMIoInterface {
        union {
            struct {
                uint8_t base_nametable: 2;
                uint8_t VRAM_addr_inc: 1;
                uint8_t sprite_ptr_table: 1;
                uint8_t bg_ptr_table: 1;
                uint8_t sprite_size: 1;
                uint8_t mode_select: 1;
                uint8_t NMI_enable: 1;
            };
            uint8_t reg = 0;
        } ctrl_reg;

        union {
            struct {
                uint8_t greyscale: 1;
                uint8_t bg_left8_enable: 1;
                uint8_t sp_left8_enable: 1;
                uint8_t bg_enable: 1;
                uint8_t sp_enable: 1;
                uint8_t red: 1;
                uint8_t green: 1;
                uint8_t blue: 1;
            };
            uint8_t reg = 0;
        } mask_reg;

        union {
            struct {
                uint8_t trash: 5;
                uint8_t sprite_overflow: 1;
                uint8_t sprite_zero_hit: 1;
                uint8_t vertical_blank: 1;
            };
            uint8_t reg = 0;
        } status_reg;

        AddressReg VRAM_addr_reg;
        AddressReg VRAM_tmp_addr_reg;
        uint8_t fine_x_scroll = 0;
//        uint8_t OAM_addr_reg = 0;


        uint16_t bg_shifter_low = 0;
        uint16_t bg_shifter_high = 0;
        uint8_t bg_attr_shifter_low = 0;
        uint8_t bg_attr_shifter_high = 0;
        uint16_t bg_next_tile = 0;
        uint8_t bg_next_attr = 0;
        uint8_t bg_next_tile_low = 0;
        uint8_t bg_next_tile_high = 0;

        int y_pos = 0;
        int x_pos = 0;

//      [ )
        const int VERT_VISIBLE_BEGIN = -1;
        const int VERT_VISIBLE_END = 240;

        const int HOR_VISIBLE_BEGIN = 1;
        const int HOR_VISIBLE_END = 257;
        const int HOR_PRERENDER_BEGIN = 321;
        const int HOR_PRERENDER_END = 337;

        NADNESS::ScreenInterface &screen;


        uint8_t PPU_read(uint16_t address);

        uint8_t get_color_from_palette(uint8_t palette, uint8_t color);

        void PPU_write(uint8_t data);

    public:
        void write_ctrl_reg(uint8_t data) override;

        void write_mask_reg(uint8_t data) override;

        uint8_t read_status_reg() override;

        void write_scroll_reg(uint8_t data) override;

        void set_OAM_address(uint8_t address) override;

        void OAM_write(uint8_t data) override;

        void set_VRAM_address(uint8_t address) override;

        void VRAM_write(uint8_t data) override;

        uint8_t VRAM_read_data() override;

        void tick();
    };


}
