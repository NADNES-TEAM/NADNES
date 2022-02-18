#include "PPU.h"
#include "screen.h"

namespace NES {
    void AddressReg::increase_x_scroll() {
        if (coarse_x_scroll == 31) {
            coarse_x_scroll = 0;
            nametable_x ^= 1;
        } else {
            coarse_x_scroll++;
        }
    }

    void AddressReg::increase_y_scroll() {
        if (fine_y_scroll < 7) {
            fine_y_scroll++;
        } else {
            fine_y_scroll = 0;
            if (coarse_y_scroll == 29) {
                coarse_y_scroll = 0;
                nametable_y ^= 1;
            } else if (coarse_y_scroll == 31) {
                coarse_y_scroll = 0;
            } else {
                coarse_y_scroll += 1;
            }
        }
    }

    void AddressReg::set_x_scroll_from(const AddressReg &other) {
        coarse_x_scroll = other.coarse_x_scroll;
        nametable_x = other.nametable_x;
    }

    void AddressReg::set_y_scroll_from(const AddressReg &other) {
        coarse_y_scroll = other.coarse_y_scroll;
        nametable_y = other.nametable_y;
        fine_y_scroll = other.fine_y_scroll;
    }

    void PPU::tick() {
        if (y_pos == 0 && x_pos == 0) x_pos = 1;  // skip odd

        if (y_pos == -1 && x_pos == 1) status_reg.vertical_blank = 0;  // clear VBlank

        if (VERT_VISIBLE_BEGIN <= y_pos && y_pos < VERT_VISIBLE_END) {
            if (HOR_VISIBLE_BEGIN <= x_pos && x_pos < HOR_VISIBLE_END ||
                HOR_PRERENDER_BEGIN <= x_pos && x_pos < HOR_PRERENDER_END) {
                int cur_step = x_pos % 8;
                if (cur_step == 0) {
                    VRAM_addr_reg.increase_x_scroll(); // inc hori(x)

                } else if (cur_step == 1) {  //NT byte
                    bg_shifter_low = (bg_shifter_low & 0xFF00) + bg_next_tile_low;
                    bg_shifter_high = (bg_shifter_high & 0xFF00) + bg_next_tile_high;
                    //TODO: load attr shifters

                    bg_next_tile = PPU_read(0x2000 + VRAM_addr_reg.reg & 0x0FFF);

                } else if (cur_step == 3) {  // AT byte
                    uint16_t address = (0x23C0 | (VRAM_addr_reg.nametable_y << 11) | (VRAM_addr_reg.nametable_x << 10) |
                                        ((VRAM_addr_reg.coarse_y_scroll / 4) * 8) |
                                        (VRAM_addr_reg.coarse_x_scroll / 4));
                    bg_next_attr = PPU_read(address);
                    if ((VRAM_addr_reg.coarse_y_scroll & 0x03) >= 2) {
                        bg_next_attr >>= 4;
                    }
                    if ((VRAM_addr_reg.coarse_x_scroll & 0x03) >= 2) {
                        bg_next_attr >>= 2;
                    }
                    bg_next_attr &= 0x03;

                } else if (cur_step == 5) {  // Low BG tile byte
                    uint16_t address = (ctrl_reg.bg_ptr_table << 12) + bg_next_tile * 16 + VRAM_addr_reg.fine_y_scroll;
                    bg_next_tile_low = PPU_read(address);

                } else if (cur_step == 7) {  // HIgh BG tile byte
                    uint16_t address =
                            (ctrl_reg.bg_ptr_table << 12) + bg_next_tile * 16 + VRAM_addr_reg.fine_y_scroll + 8;
                    bg_next_tile_high = PPU_read(address);
                }
            }

            if (x_pos == 256) {
                VRAM_addr_reg.increase_y_scroll(); // inc vert(v)
            }

            if (x_pos == 257) {
                VRAM_addr_reg.set_x_scroll_from(VRAM_tmp_addr_reg);  // hori(v) = hori(t)
            }

            if (x_pos == 337 || x_pos == 339) {  // useless NT bytes at the end of scanline
                bg_next_tile = PPU_read(0x2000 + VRAM_addr_reg.reg & 0x0FFF);
            }

            if (y_pos == -1 || x_pos >= 280 || x_pos <= 304) {
                VRAM_addr_reg.set_y_scroll_from(VRAM_tmp_addr_reg); // vert(v) = vert(t)
            }


        } else if (y_pos == 241 && x_pos == 1) {  // set VBlank flag
            status_reg.vertical_blank = 1;
            if (ctrl_reg.NMI_enable) {
                //TODO: actually call NMI
            }
        }

        uint8_t cur_palette_bits = 0x00;
        uint8_t cur_color_bits = 0x00;
        if (mask_reg.bg_enable) {
            uint8_t cur_pixel_low_bit = ((bg_shifter_low << fine_x_scroll) & 0x8000) == 0x8000;
            uint8_t cur_pixel_high_bit = ((bg_shifter_high << fine_x_scroll) & 0x8000) == 0x8000;
            cur_color_bits = cur_pixel_high_bit * 2 + cur_pixel_low_bit;

            uint8_t cur_palette_low_bit = ((bg_shifter_low << fine_x_scroll) & 0x8000) == 0x8000;
            uint8_t cur_palette_high_bit = ((bg_shifter_high << fine_x_scroll) & 0x8000) == 0x8000;
            cur_palette_bits = cur_palette_high_bit * 2 + cur_pixel_low_bit;


        }

        screen.set_pixel(y_pos, x_pos, get_color_from_palette(cur_palette_bits, cur_color_bits));

        if (++x_pos >= 341) {
            x_pos = 0;
            if (++y_pos >= 261) {
                y_pos = -1;
            }
        }
    }


}