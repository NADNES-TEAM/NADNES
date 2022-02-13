#ifndef NADNES_MAPPER_H
#define NADNES_MAPPER_H

#include <cstdint>
#include "nes_exceptions.h"

namespace NES {
    enum class Mirroring {
        Horizontal, Vertical
    };

    struct AbstractMapper {
        virtual uint16_t map_PRG_ROM_address(uint16_t address) = 0;

        virtual uint16_t map_CHR_ROM_address(uint16_t address) = 0;

        virtual ~AbstractMapper() = default;
    };



}  // namespace NES

#endif  // NADNES_MAPPER_H
