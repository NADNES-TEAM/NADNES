#pragma once

#include <cstdint>
#include <string>
#include "mapper.h"
#include <stdexcept>
#include <memory>
#include <vector>

namespace NES {
    struct CpuToCartridgeInterface {
        [[nodiscard]] virtual uint8_t CPU_read(uint16_t address) const = 0;

        virtual void CPU_write(uint16_t address, uint8_t data) = 0;

        virtual ~CpuToCartridgeInterface() = default;
    };

    struct PpuToCartridgeInterface {
        [[nodiscard]] virtual uint8_t PPU_read(uint16_t address) const = 0;

        virtual void PPU_write(uint16_t address, uint8_t data) = 0;

        virtual ~PpuToCartridgeInterface() = default;
    };

    struct iNesHeader {
        uint32_t id_str = 0;  // 0-3
        uint8_t PRG_ROM_banks_lsb = 0; // 4
        uint8_t CHR_ROM_banks_lsb = 0; // 5
        struct {  // 6
            uint8_t mirror_type: 1;
            uint8_t non_volatile_mem: 1;
            uint8_t trainer: 1;
            uint8_t four_screen_mode: 1;
            uint8_t mapper_low_bits: 4;
        };
        struct { // 7
            uint8_t console_type: 2;
            uint8_t nes_2_0_id: 2;
            uint8_t mapper_high_bits: 4;
        };
        struct { // 8
            uint8_t mapper_extra_bits: 4;
            uint8_t submapper_num: 4;
        };
        struct { // 9
            uint8_t PRG_ROM_size_msb: 4;
            uint8_t CHR_ROM_size_msb: 4;
        };
        uint8_t unused_byte_10 = 0;
        struct { // 11
            uint8_t CHR_RAM_size: 4;
            uint8_t CHR_NVRAM_size: 4;
        };
        uint32_t unused_last_bytes = 0;
    };


    class Cartridge : public CpuToCartridgeInterface, public PpuToCartridgeInterface {
        std::unique_ptr<AbstractMapper> mapper;
        std::vector<uint8_t> PRG_ROM;
        std::vector<uint8_t> CHR_ROM;
        std::vector<uint8_t> CHR_RAM;

        [[nodiscard]] uint8_t CPU_read(uint16_t address) const override;

        void CPU_write(uint16_t address, uint8_t data) override;

        [[nodiscard]] uint8_t PPU_read(uint16_t address) const override;

        void PPU_write(uint16_t address, uint8_t data) override;

    public:
        explicit Cartridge(const std::string &filename);
    };


}  // namespace NES
