#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "all_nes_fwd.h"
#include "cartridge.h"
#include "connect_token.h"
#include "interfaces/screen_interface.h"
#include "reset_token.h"

namespace NES {

#pragma pack(push, 1)

class AddressReg {
    friend class Ppu;

    void increase_y_scroll();

    void increase_x_scroll();

    void set_y_scroll_from(const AddressReg &other);

    void set_x_scroll_from(const AddressReg &other);

    union {
        struct {
            uint16_t coarse_x_scroll  : 5;
            uint16_t coarse_y_scroll  : 5;
            uint16_t nametable_x      : 1;
            uint16_t nametable_y      : 1;
            uint16_t fine_y_scroll    : 3;
            uint16_t unused_loopy_bit : 1;
        };
        struct {
            uint16_t address_low_byte  : 8;
            uint16_t address_high_byte : 6;
            uint16_t z_bit             : 1;
            uint16_t unused_addr_bit   : 1;
        };
        uint16_t reg = 0;
    };
};

struct SpriteInfo {
    uint8_t y_coord = 0xFF;
    uint8_t tile_index = 0xFF;
    uint8_t attribute = 0xFF;
    uint8_t x_coord = 0xFF;
};

struct SpriteData {
    uint8_t shifter_high = 0;
    uint8_t shifter_low = 0;
    uint8_t counter = 0;
    union {
        struct {
            uint8_t palette         : 2;
            uint8_t unused          : 3;
            uint8_t priority        : 1;
            uint8_t flip_horizontal : 1;
            uint8_t flip_vertical   : 1;
        };
        uint8_t attribute = 0;
    };

    uint8_t get_color();
};

struct PpuData {
    // registers
    union {
        struct {
            uint8_t base_nametable_x : 1;
            uint8_t base_nametable_y : 1;
            uint8_t VRAM_addr_inc    : 1;
            uint8_t sprite_ptr_table : 1;
            uint8_t bg_ptr_table     : 1;
            uint8_t sprite_size      : 1;
            uint8_t mode_select      : 1;
            uint8_t NMI_enable       : 1;
        };
        uint8_t reg = 0;
    } ctrl_reg;

    union {
        struct {
            uint8_t greyscale       : 1;
            uint8_t bg_left8_enable : 1;
            uint8_t sp_left8_enable : 1;
            uint8_t bg_enable       : 1;
            uint8_t sp_enable       : 1;
            uint8_t red             : 1;
            uint8_t green           : 1;
            uint8_t blue            : 1;
        };
        uint8_t reg = 0;
    } mask_reg;

    union {
        struct {
            uint8_t trash           : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank  : 1;
        };
        uint8_t reg = 0;
    } status_reg;

    AddressReg VRAM_addr_reg{};
    AddressReg VRAM_tmp_addr_reg{};
    uint8_t fine_x_scroll = 0;
    bool double_write_toggle = false;
    uint8_t VRAM_read_buff = 0;

    // background rendering state
    bool is_rendering = false;
    uint16_t bg_shifter_low = 0;
    uint16_t bg_shifter_high = 0;
    uint8_t bg_cur_palette = 0;
    uint8_t bg_next_palette = 0;
    uint8_t bg_fetched_palette = 0;
    uint16_t bg_next_tile_num = 0;
    uint8_t bg_next_tile_low = 0;
    uint8_t bg_next_tile_high = 0;
    int y_pos = 0;
    int x_pos = 0;
    bool odd_frame = false;

    // sprites rendering state
    union {
        struct {
            uint8_t sprite_eval_m : 2;
            uint8_t sprite_eval_n : 6;
        };
        uint8_t OAM_addr_reg = 0;
    };

    int OAM_clearing_counter = 0;
    bool OAM_is_busy = false;
    int detected_sprites = 0;
    bool sprite_detection_complete = false;
    int sp_fetch_count = 0;
    bool sprite_zero_next_line = false;
    bool sprite_zero_cur_line = false;
};

class Ppu : PpuData {
    // background rendering constants [ )
    constexpr static int VERT_VISIBLE_BEGIN = -1;
    constexpr static int VERT_VISIBLE_END = 240;
    constexpr static int HOR_VISIBLE_BEGIN = 1;
    constexpr static int HOR_VISIBLE_END = 257;
    constexpr static int HOR_PRERENDER_BEGIN = 321;
    constexpr static int HOR_PRERENDER_END = 337;

    // sprite evaluation constants [ )
    constexpr static int OAM_CLEAR_BEGIN = 1;
    constexpr static int OAM_CLEAR_END = 65;
    constexpr static int SP_DETECT_BEGIN = 65;
    constexpr static int SP_DETECT_END = 257;
    constexpr static int SP_FETCH_BEGIN = 257;
    constexpr static int SP_FETCH_END = 321;

    // memory
    std::vector<SpriteInfo> secondary_OAM;
    std::vector<SpriteData> loaded_sprites;
    std::vector<uint8_t> OAM;
    std::vector<uint8_t> palette_mem;

    //  connected devices
    ScreenInterface *screen = nullptr;
    PpuToCartridgeInterface *cartridge = nullptr;
    Cpu *cpu = nullptr;

    [[nodiscard]] uint8_t PPU_read(uint16_t address) const;

    void PPU_write(uint16_t address, uint8_t data);

public:
    void connect(PpuToCartridgeInterface *cartridge_, ConnectToken) noexcept;

    void connect(ScreenInterface *screen_, ConnectToken) noexcept;

    void connect(Cpu *cpu_, ConnectToken) noexcept;

    [[nodiscard]] explicit Ppu();

    void reset(ResetToken);

    void write_ctrl_reg(uint8_t data);

    void write_mask_reg(uint8_t data);

    uint8_t read_status_reg();

    void write_scroll_reg(uint8_t data);

    void set_OAM_address(uint8_t address);

    void OAM_write(uint8_t data);

    [[nodiscard]] uint8_t OAM_read() const;

    void set_VRAM_address(uint8_t address);

    void VRAM_write(uint8_t data);

    uint8_t VRAM_read();

    bool tick();

    void save(std::ostream &file);

    void load(std::istream &file);
};
#pragma pack(pop)
}  // namespace NES
