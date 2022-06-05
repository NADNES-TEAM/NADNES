#pragma once
#include "interfaces/mapper.h"

namespace NES {

struct UnromMapper : AbstractMapper {
    Mirroring mirror_type;
    uint8_t low_bank_select = 0;
    uint8_t high_bank_select;

    UnromMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr);

    void map_write_from_CPU(uint16_t address, uint8_t data) override;

    [[nodiscard]] uint32_t map_read_from_CPU(uint16_t address) const override;

    [[nodiscard]] uint16_t map_PPU_address(uint16_t address) const override;

    void save(std::ostream &file) override;

    void load(std::istream &file) override;

    void reset(ResetToken) override;
};

}  // namespace NES
