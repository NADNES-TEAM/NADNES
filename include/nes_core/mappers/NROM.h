#pragma once

#include "interfaces/mapper.h"

namespace NES {

struct NromMapper : AbstractMapper {
    Mirroring mirror_type;
    uint8_t PRG_banks_count;
    uint8_t CHR_banks_count;

    NromMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr);

    [[nodiscard]] uint32_t map_read_from_CPU(uint16_t address) const override;

    [[nodiscard]] uint16_t map_PPU_address(uint16_t address) const override;
};

}  // namespace NES
