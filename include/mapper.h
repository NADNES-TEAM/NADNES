#ifndef NADNES_MAPPER_H
#define NADNES_MAPPER_H
#include <cstdint>

namespace NES {
    struct mapper_PPU_interface {
        virtual uint8_t read(uint16_t address) = 0;
    };

    struct mapper_CPU_interface {
        virtual uint8_t read(uint16_t address) = 0;
    };


}

#endif //NADNES_MAPPER_H
