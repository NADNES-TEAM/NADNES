#include "nes_devices/ppu.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "interfaces/screen_interface.h"
#include "nes_devices/R6502.h"

namespace NES {

const std::vector<uint8_t> reverse = []() {
    std::vector<uint8_t> d(256);
    for (int i = 1; i < 256; i++) {
        d[i] = (d[i >> 1] >> 1) + ((i & 1) << 7);
    }
    return d;
}();

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
            coarse_y_scroll++;
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

uint8_t SpriteData::get_color() {
    if (counter == 0) {
        int color = ((shifter_high & 0x80) == 0x80) * 2 + ((shifter_low & 0x80) == 0x80);
        shifter_high <<= 1;
        shifter_low <<= 1;
        return color;
    } else {
        counter--;
    }
    return 0;
}

std::vector<Color> colors = {
    {84, 84, 84},    {0, 30, 116},    {8, 16, 144},    {48, 0, 136},    {68, 0, 100},
    {92, 0, 48},     {84, 4, 0},      {60, 24, 0},     {32, 42, 0},     {8, 58, 0},
    {0, 64, 0},      {0, 60, 0},      {0, 50, 60},     {0, 0, 0},       {0, 0, 0},
    {0, 0, 0},       {152, 150, 152}, {8, 76, 196},    {48, 50, 236},   {92, 30, 228},
    {136, 20, 176},  {160, 20, 100},  {152, 34, 32},   {120, 60, 0},    {84, 90, 0},
    {40, 114, 0},    {8, 124, 0},     {0, 118, 40},    {0, 102, 120},   {0, 0, 0},
    {0, 0, 0},       {0, 0, 0},       {236, 238, 236}, {76, 154, 236},  {120, 124, 236},
    {176, 98, 236},  {228, 84, 236},  {236, 88, 180},  {236, 106, 100}, {212, 136, 32},
    {160, 170, 0},   {116, 196, 0},   {76, 208, 32},   {56, 204, 108},  {56, 180, 204},
    {60, 60, 60},    {0, 0, 0},       {0, 0, 0},       {236, 238, 236}, {168, 204, 236},
    {188, 188, 236}, {212, 178, 236}, {236, 174, 236}, {236, 174, 212}, {236, 180, 176},
    {228, 196, 144}, {204, 210, 120}, {180, 222, 120}, {168, 226, 144}, {152, 226, 180},
    {160, 214, 228}, {160, 162, 160}, {0, 0, 0},       {0, 0, 0}};

bool Ppu::tick() {
    is_rendering = false;

    if (y_pos == -1 && x_pos == 1) {
        status_reg.vertical_blank = 0;  // clear VBlank
        status_reg.sprite_overflow = false;
        status_reg.sprite_zero_hit = false;
    }

    if (VERT_VISIBLE_BEGIN <= y_pos && y_pos < VERT_VISIBLE_END) {
        if (mask_reg.bg_enable || mask_reg.sp_enable) {
            if (y_pos == 0 && x_pos == 0 && odd_frame) {
                x_pos = 1;  // skip odd
            }
            is_rendering = true;
            if (HOR_VISIBLE_BEGIN <= x_pos && x_pos < HOR_VISIBLE_END ||
                HOR_PRERENDER_BEGIN <= x_pos && x_pos < HOR_PRERENDER_END) {
                // actual shifting
                if (mask_reg.bg_enable) {
                    bg_shifter_low <<= 1;
                    bg_shifter_high <<= 1;
                }

                int cur_step = x_pos % 8;

                if (cur_step == 0 && x_pos != 0) {
                    VRAM_addr_reg.increase_x_scroll();  // inc hori(x)

                } else if (cur_step == 1) {  // load shifters & NT byte
                    bg_shifter_low = (bg_shifter_low & 0xFF00) + bg_next_tile_low;
                    bg_shifter_high = (bg_shifter_high & 0xFF00) + bg_next_tile_high;
                    bg_cur_palette = bg_next_palette;
                    bg_next_palette = bg_fetched_palette;

                    bg_next_tile_num = PPU_read(0x2000 + (VRAM_addr_reg.reg & 0x0FFF));
                    //                    std::cout << std::setw(3) << std::setfill('0') <<
                    //                    (int)bg_next_tile_num << ' ';

                } else if (cur_step == 3) {  // AT byte
                    uint16_t address = (0x23C0 | (VRAM_addr_reg.nametable_y << 11) |
                                        (VRAM_addr_reg.nametable_x << 10) |
                                        ((VRAM_addr_reg.coarse_y_scroll / 4) * 8) |
                                        (VRAM_addr_reg.coarse_x_scroll / 4));
                    bg_fetched_palette = PPU_read(address);
                    if ((VRAM_addr_reg.coarse_y_scroll & 0x03) >= 2) {
                        bg_fetched_palette >>= 4;
                    }
                    if ((VRAM_addr_reg.coarse_x_scroll & 0x03) >= 2) {
                        bg_fetched_palette >>= 2;
                    }
                    bg_fetched_palette &= 0x03;

                } else if (cur_step == 5) {  // Low BG tile byte
                    uint16_t address = ((ctrl_reg.bg_ptr_table << 12) + bg_next_tile_num * 16 +
                                        VRAM_addr_reg.fine_y_scroll);
                    bg_next_tile_low = PPU_read(address);

                } else if (cur_step == 7) {  // High BG tile byte
                    uint16_t address = ((ctrl_reg.bg_ptr_table << 12) + bg_next_tile_num * 16 +
                                        VRAM_addr_reg.fine_y_scroll + 8);
                    bg_next_tile_high = PPU_read(address);
                }
            }

            if (x_pos == 256) {
                VRAM_addr_reg.increase_y_scroll();  // inc vert(v)
            }

            if (x_pos == 257) {
                VRAM_addr_reg.set_x_scroll_from(VRAM_tmp_addr_reg);  // hori(v) = hori(t)
            }

            if (x_pos == 337 || x_pos == 339) {  // useless NT bytes at the end of scanline
                bg_next_tile_num = PPU_read(0x2000 + VRAM_addr_reg.reg & 0x0FFF);
            }

            if (y_pos == -1 && x_pos >= 280 && x_pos <= 304) {
                VRAM_addr_reg.set_y_scroll_from(VRAM_tmp_addr_reg);  // vert(v) = vert(t)
            }

            // sprites
            // clearing secondary OAM
            if (y_pos != -1 && x_pos == OAM_CLEAR_BEGIN) {
                OAM_clearing_counter = 0;
                OAM_is_busy = true;
            }
            if (y_pos != -1 && OAM_CLEAR_BEGIN <= x_pos && x_pos < OAM_CLEAR_END &&
                x_pos % 8 == 0) {
                secondary_OAM[OAM_clearing_counter++] = {};
            }

            // detecting sprites for next line
            if (y_pos != -1 && x_pos == SP_DETECT_BEGIN) {
                OAM_is_busy = false;
                detected_sprites = 0;
                sprite_detection_complete = false;
                OAM_addr_reg = 0;
            }
            if (y_pos != -1 && SP_DETECT_BEGIN <= x_pos && x_pos < SP_DETECT_END && x_pos % 2 &&
                detected_sprites <= 8 && !sprite_detection_complete) {
                int dy = y_pos - OAM[OAM_addr_reg];
                if (0 <= dy && dy < (8 + 8 * ctrl_reg.sprite_size)) {
                    if (detected_sprites < 8) {
                        sprite_zero_next_line |= sprite_eval_n == 0;
                        secondary_OAM[detected_sprites] = {OAM[OAM_addr_reg],
                                                           OAM[OAM_addr_reg + 1],
                                                           OAM[OAM_addr_reg + 2],
                                                           OAM[OAM_addr_reg + 3]};
                        sprite_eval_n++;
                    } else {
                        status_reg.sprite_overflow = true;
                        OAM_addr_reg += 3;
                        if (sprite_eval_m == 3) {  // TODO:
                            sprite_eval_n += 1;
                        }
                    }
                    detected_sprites++;
                } else {
                    if (detected_sprites == 8) {
                        sprite_eval_m++;
                    }
                    sprite_eval_n++;
                }
                sprite_detection_complete = sprite_eval_n == 0;
            }

            // loading data for detected sprites
            if (x_pos == SP_FETCH_BEGIN) {
                sp_fetch_count = 0;
            }
            if (SP_FETCH_BEGIN <= x_pos && x_pos < SP_FETCH_END && (x_pos % 4 == 0) &&
                sp_fetch_count < 8) {
                SpriteData cur_sp;
                if (sp_fetch_count < detected_sprites) {
                    cur_sp.attribute = secondary_OAM[sp_fetch_count].attribute;
                    cur_sp.counter = secondary_OAM[sp_fetch_count].x_coord;
                    uint8_t tile_index = secondary_OAM[sp_fetch_count].tile_index;
                    uint16_t addr = 0;
                    int dy = y_pos - secondary_OAM[sp_fetch_count].y_coord;
                    if (ctrl_reg.sprite_size == 0) {  // 8x8
                        addr = 0x1000 * ctrl_reg.sprite_ptr_table + tile_index * 16 + dy ^
                               (7 * cur_sp.flip_vertical);
                    } else {                               // 8x16
                        addr = 0x1000 * (tile_index & 1);  // nametable
                        // tile
                        addr += ((tile_index / 2 * 2) + ((dy >= 8) ^ cur_sp.flip_vertical)) * 16;
                        addr += (dy % 8) ^ (7 * cur_sp.flip_vertical);  // row
                    }
                    uint8_t fetched_high = PPU_read(addr + 8);
                    uint8_t fetched_low = PPU_read(addr);
                    cur_sp.shifter_high =
                        cur_sp.flip_horizontal ? reverse[fetched_high] : fetched_high;
                    cur_sp.shifter_low =
                        cur_sp.flip_horizontal ? reverse[fetched_low] : fetched_low;
                }
                loaded_sprites[sp_fetch_count++] = cur_sp;
            }
        }
    }

    if (y_pos == 241 && x_pos == 1) {  // set VBlank flag
        status_reg.vertical_blank = 1;
        if (ctrl_reg.NMI_enable) {
            cpu->NMI();
        }
    }

    uint8_t bg_color = 0x00;
    if (mask_reg.bg_enable) {
        uint8_t cur_pixel_low_bit = ((bg_shifter_low << fine_x_scroll) & 0x8000) == 0x8000;
        uint8_t cur_pixel_high_bit = ((bg_shifter_high << fine_x_scroll) & 0x8000) == 0x8000;
        bg_color = cur_pixel_high_bit * 2 + cur_pixel_low_bit;
    }
    uint8_t sp_color = 0x00;
    uint8_t sp_palette = 0x00;
    bool sp_priority = false;
    bool sprite_zero_chosen = false;
    if (mask_reg.sp_enable && x_pos >= 1 && x_pos < 257) {
        for (int i = 0; i < 8; i++) {
            uint8_t color = loaded_sprites[i].get_color();
            if (color && (sp_color == 0x00)) {
                sp_color = color;
                sp_palette = loaded_sprites[i].palette;
                sp_priority = loaded_sprites[i].priority;
                if (i == 0) {
                    sprite_zero_chosen = true;
                }
            }
        }
    }
    uint8_t cur_palette = 0;
    uint8_t cur_color = 0;
    if (sp_color || bg_color) {
        if ((sp_color == 0) || (bg_color != 0 && sp_priority)) {
            cur_palette =
                (((x_pos - 1) % 8 + fine_x_scroll) > 7) ? bg_next_palette : bg_cur_palette;
            cur_palette *= 4;
            cur_color = bg_color;
        } else {
            cur_palette = sp_palette * 4 + 0x10;
            cur_color = sp_color;
        }
    }
    bool left_8_enable = mask_reg.bg_left8_enable && mask_reg.sp_left8_enable;
    if (sprite_zero_cur_line && sprite_zero_chosen && bg_color != 0 && sp_color != 0) {
        status_reg.sprite_zero_hit |=
            (x_pos >= (1 + 8 * !left_8_enable)) && (x_pos <= HOR_VISIBLE_END) && (x_pos != 255);
    }

    screen->set_pixel(y_pos, x_pos, colors[PPU_read(0x3F00 + cur_palette + cur_color)]);

    if (++x_pos >= 341) {
        x_pos = 0;
        sprite_zero_cur_line = sprite_zero_next_line;
        if (++y_pos >= 261) {
            screen->refresh_screen();
            y_pos = -1;
            odd_frame ^= 1;
            return true;
        }
    }
    return false;
}

void Ppu::write_ctrl_reg(uint8_t data) {
    bool prev_NMI_enable_state = ctrl_reg.NMI_enable;
    ctrl_reg.reg = data;
    VRAM_tmp_addr_reg.nametable_x = ctrl_reg.base_nametable_x;
    VRAM_tmp_addr_reg.nametable_y = ctrl_reg.base_nametable_y;
    if (!prev_NMI_enable_state && ctrl_reg.NMI_enable && status_reg.vertical_blank) {
        cpu->NMI();
    }  // TODO: check this feature
}

void Ppu::write_mask_reg(uint8_t data) {
    mask_reg.reg = data;
}

uint8_t Ppu::read_status_reg() {
    uint8_t reg_state = status_reg.reg;
    status_reg.vertical_blank = 0;
    double_write_toggle = false;
    return reg_state;
}

void Ppu::write_scroll_reg(uint8_t data) {
    if (!double_write_toggle) {
        VRAM_tmp_addr_reg.coarse_x_scroll = (data >> 3);
        fine_x_scroll = (data & 0x07);
    } else {
        VRAM_tmp_addr_reg.coarse_y_scroll = (data >> 3);
        VRAM_tmp_addr_reg.fine_y_scroll = (data & 0x07);
    }
    double_write_toggle ^= 1;
}

void Ppu::set_VRAM_address(uint8_t address) {
    if (!double_write_toggle) {
        VRAM_tmp_addr_reg.address_high_byte = (address & 0x3f);
        VRAM_tmp_addr_reg.z_bit = 0;
    } else {
        VRAM_tmp_addr_reg.address_low_byte = address;
        VRAM_addr_reg = VRAM_tmp_addr_reg;
    }
    double_write_toggle ^= 1;
}

void Ppu::VRAM_write(uint8_t data) {
    PPU_write(VRAM_addr_reg.reg, data);
    if (is_rendering) {
        VRAM_addr_reg.increase_y_scroll();
        VRAM_addr_reg.increase_x_scroll();
    } else {
        VRAM_addr_reg.reg += (ctrl_reg.VRAM_addr_inc ? 32 : 1);
    }
}

uint8_t Ppu::VRAM_read() {
    uint8_t ret = VRAM_read_buff;
    VRAM_read_buff = PPU_read(VRAM_addr_reg.reg);
    if ((VRAM_addr_reg.reg % 0x4000) >= 0x3F00) {  // palette read
        ret = VRAM_read_buff;
    }
    if (is_rendering) {
        VRAM_addr_reg.increase_y_scroll();
        VRAM_addr_reg.increase_x_scroll();
    } else {
        VRAM_addr_reg.reg += (ctrl_reg.VRAM_addr_inc ? 32 : 1);
    }
    return ret;
}

uint8_t Ppu::PPU_read(uint16_t address) const {
    address %= 0x4000;
    if (address < 0x3F00) {
        return cartridge->PPU_read(address);
    } else if (address >= 0x3F00) {
        address %= 0x20;
        if (address % 4 == 0)
            address %= 0x10;
        return palette_mem[address];
    }
    return 0;
}

void Ppu::PPU_write(uint16_t address, uint8_t data) {
    address %= 0x4000;
    if (address < 0x3F00) {
        cartridge->PPU_write(address, data);
    } else if (address >= 0x3F00) {
        address %= 0x20;
        if (address % 4 == 0)
            address %= 0x10;
        palette_mem[address] = data;
    }
}

void Ppu::connect(PpuToCartridgeInterface *cartridge_, ConnectToken) noexcept {
    cartridge = cartridge_;
}

void Ppu::connect(ScreenInterface *screen_, ConnectToken) noexcept {
    screen = screen_;
}

Ppu::Ppu() : OAM(256), secondary_OAM(32), palette_mem(32), loaded_sprites(8) {}

void Ppu::set_OAM_address(uint8_t address) {
    OAM_addr_reg = address;
}

void Ppu::OAM_write(uint8_t data) {
    OAM[OAM_addr_reg++] = data;
}

uint8_t Ppu::OAM_read() const {
    return OAM_is_busy ? 0xFF : OAM[OAM_addr_reg];
}

void Ppu::connect(Cpu *cpu_, ConnectToken) noexcept {
    cpu = cpu_;
}

void Ppu::reset(ResetToken) {
    ctrl_reg.reg = 0;
    mask_reg.reg = 0;
    VRAM_tmp_addr_reg.reg = 0;
    double_write_toggle = false;
    VRAM_read_buff = 0;
    odd_frame = false;
}

void Ppu::save(std::ofstream &file) {
    file.write(reinterpret_cast<char *>(this), sizeof(PpuData));
    file.write(reinterpret_cast<char *>(&OAM[0]), OAM.size());
    file.write(reinterpret_cast<char *>(&palette_mem[0]), palette_mem.size());
    file.write(reinterpret_cast<char *>(&secondary_OAM[0]),
               secondary_OAM.size() * sizeof(secondary_OAM[0]));
    file.write(reinterpret_cast<char *>(&loaded_sprites[0]),
               loaded_sprites.size() * sizeof(loaded_sprites[0]));
}

void Ppu::load(std::ifstream &file) {
    file.read(reinterpret_cast<char *>(this), sizeof(PpuData));
    file.read(reinterpret_cast<char *>(&OAM[0]), OAM.size());
    file.read(reinterpret_cast<char *>(&palette_mem[0]), palette_mem.size());
    file.read(reinterpret_cast<char *>(&secondary_OAM[0]),
              secondary_OAM.size() * sizeof(secondary_OAM[0]));
    file.read(reinterpret_cast<char *>(&loaded_sprites[0]),
              loaded_sprites.size() * sizeof(loaded_sprites[0]));
}

}  // namespace NES
