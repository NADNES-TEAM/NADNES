#pragma once

#include <cstdint>
#include <iosfwd>
#include "nes_core/nes_exceptions.h"
#include "nes_core/reset_token.h"

namespace NES {
enum class Mirroring { Horizontal, Vertical };

constexpr uint16_t PRG_ROM_BANK_SIZE = 1 << 14;
constexpr uint16_t CHR_ROM_BANK_SIZE = 1 << 13;
constexpr uint16_t CHR_RAM_size = 1 << 12;

struct AbstractMapper {
    [[nodiscard]] virtual uint32_t map_read_from_CPU(uint16_t address) const = 0;

    virtual void map_write_from_CPU(uint16_t address, uint8_t data){};

    [[nodiscard]] virtual uint16_t map_PPU_address(uint16_t address) const = 0;

    virtual void reset(ResetToken){};

    virtual void save(std::ostream &file) {}

    virtual void load(std::istream &file) {}

    virtual ~AbstractMapper() = default;
};

}  // namespace NES
