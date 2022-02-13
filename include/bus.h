#ifndef BUS_H
#define BUS_H
#include <cstdint>
namespace NES {
    struct BusInterface {
        virtual void mem_write(uint16_t addr, uint8_t data) = 0;

        virtual uint8_t mem_read(uint16_t addr) = 0;
    };

    struct Bus : BusInterface {
        CPU cpu;
        std::array<uint8_t, 1<<13> RAM{};
        std::array<uint8_t, 1<<13> SRAM{};
        uint8_t mem_read(uint16_t addr) override {
            return 0;
        }
        void mem_write(uint16_t addr, uint8_t data) override {}
    };
}
#endif //BUS_H
