#ifndef NADNES_MAPPER_H
#define NADNES_MAPPER_H
#include <cstdint>

namespace NES {
    struct MapperPPUInterface {
        virtual uint8_t read(uint16_t address) = 0;
    };

    struct MapperCPUInterface {
        virtual uint8_t read(uint16_t address) = 0;
    };


}

#endif //NADNES_MAPPER_H
