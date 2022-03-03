#pragma once


#include "mapper.h"

namespace NES {
struct NROMMapper : AbstractMapper {
    Mirroring mirror_type;
    uint8_t PRG_banks_count;
    uint8_t CHR_banks_count;

    NROMMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr);

    [[nodiscard]] uint16_t map_read_from_CPU(uint16_t address) const override;

    [[nodiscard]] uint16_t map_PPU_address(uint16_t address) const override;
};

}  // namespace NES
