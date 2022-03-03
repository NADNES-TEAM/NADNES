#ifndef BUS_H
#define BUS_H
#include <cstdint>
#include <array>
namespace NES {
    struct Bus {
        std::array<uint8_t, 1<<13> RAM{};
        std::array<uint8_t, 1<<13> SRAM{};
        uint8_t mem_read(uint16_t addr);
        void mem_write(uint16_t addr, uint8_t data);
    };
}
#endif //BUS_H
