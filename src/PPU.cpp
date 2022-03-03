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
    if (mask_reg.bg_enable || mask_reg.sp_enable) {
        if (y_pos == 0 && x_pos == 0)
            x_pos = 1;  // skip odd

        if (y_pos == -1 && x_pos == 1)
            status_reg.vertical_blank = 0;  // clear VBlank

        if (VERT_VISIBLE_BEGIN <= y_pos && y_pos < VERT_VISIBLE_END) {
            is_rendering = true;
            if (HOR_VISIBLE_BEGIN <= x_pos && x_pos < HOR_VISIBLE_END ||
                HOR_PRERENDER_BEGIN <= x_pos && x_pos < HOR_PRERENDER_END) {
                int cur_step = x_pos % 8;
                if (cur_step == 0) {
                    VRAM_addr_reg.increase_x_scroll();  // inc hori(x)

                } else if (cur_step == 1) {  // update shifters & NT byte
                    bg_shifter_low = (bg_shifter_low & 0xFF00) + bg_next_tile_low;
                    bg_shifter_high = (bg_shifter_high & 0xFF00) + bg_next_tile_high;
                    bg_cur_palette = bg_next_palette;

                    bg_next_tile_num = PPU_read(0x2000 + VRAM_addr_reg.reg & 0x0FFF);

                } else if (cur_step == 3) {  // AT byte
                    uint16_t address = (0x23C0 | (VRAM_addr_reg.nametable_y << 11) |
                                        (VRAM_addr_reg.nametable_x << 10) |
                                        ((VRAM_addr_reg.coarse_y_scroll / 4) * 8) |
                                        (VRAM_addr_reg.coarse_x_scroll / 4));
                    bg_next_palette = PPU_read(address);
                    if ((VRAM_addr_reg.coarse_y_scroll & 0x03) >= 2) {
                        bg_next_palette >>= 4;
                    }
                    if ((VRAM_addr_reg.coarse_x_scroll & 0x03) >= 2) {
                        bg_next_palette >>= 2;
                    }
                    bg_next_palette &= 0x03;

                } else if (cur_step == 5) {  // Low BG tile byte
                    uint16_t address = ((ctrl_reg.bg_ptr_table << 12) + bg_next_tile_num * 16 +
                                        VRAM_addr_reg.fine_y_scroll);
                    bg_next_tile_low = PPU_read(address);

                } else if (cur_step == 7) {  // HIgh BG tile byte
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

            if (y_pos == -1 || x_pos >= 280 || x_pos <= 304) {
                VRAM_addr_reg.set_y_scroll_from(VRAM_tmp_addr_reg);  // vert(v) = vert(t)
            }

        } else if (y_pos == 241 && x_pos == 1) {  // set VBlank flag
            status_reg.vertical_blank = 1;
            if (ctrl_reg.NMI_enable) {
                // TODO: actually call NMI
            }
        }
    }

    uint8_t bg_color = 0x00;
    if (mask_reg.bg_enable) {
        uint8_t cur_pixel_low_bit = ((bg_shifter_low << fine_x_scroll) & 0x8000) == 0x8000;
        uint8_t cur_pixel_high_bit = ((bg_shifter_high << fine_x_scroll) & 0x8000) == 0x8000;
        bg_color = cur_pixel_high_bit * 2 + cur_pixel_low_bit;
    }

    screen->set_pixel(y_pos, x_pos, get_color_from_palette(bg_cur_palette, bg_color));
    if (++x_pos >= 341) {
        x_pos = 0;

        if (++y_pos >= 261) {
            screen->refresh_screen();
            //            test_utility();

            y_pos = -1;
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
        // TODO: call NMI
    }
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
        VRAM_tmp_addr_reg.coarse_x_scroll = (data >> 2);
        fine_x_scroll = (data & 0x07);
    } else {
        VRAM_tmp_addr_reg.coarse_y_scroll = (data >> 2);
        VRAM_tmp_addr_reg.fine_y_scroll = (data & 0x07);
    }
    double_write_toggle ^= 1;
}

void Ppu::set_VRAM_address(uint8_t address) {
    if (!double_write_toggle) {
        VRAM_tmp_addr_reg.address_high_byte = ((address << 2) >> 2);
        VRAM_tmp_addr_reg.z_bit = 0;
    } else {
        VRAM_addr_reg.address_low_byte = address;
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
Color Ppu::get_color_from_palette(uint8_t palette, uint8_t color) const {
    return colors[PPU_read(0x3F00 + palette * 4 + color)];
}

void Ppu::connect(ScreenInterface *screen_, ConnectToken) noexcept {
    screen = screen_;
}

Ppu::Ppu() : OAM(256), palette_mem(32) {}

}  // namespace NES