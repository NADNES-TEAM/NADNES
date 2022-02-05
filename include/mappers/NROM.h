#ifndef NADNES_NROM_H
#define NADNES_NROM_H

#include "mapper.h"

namespace NES {
    struct NROMMapper : AbstractMapper {
        Mirroring mirror_type;
        uint8_t PRG_banks_count;
        uint8_t CHR_banks_count;

        NROMMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr);

        uint16_t map_PRG_ROM_address(uint16_t address) override;

        uint16_t map_CHR_ROM_address(uint16_t address) override;
    };

}

#endif //NADNES_NROM_H
