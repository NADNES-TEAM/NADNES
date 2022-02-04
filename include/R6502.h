#ifndef R6502_H
#define R6502_H
#include <cstdint>

namespace NES {
    class CPU {
        union {
            struct {
                bool CF: 1;
                bool ZF: 1;
                bool ID: 1;
                bool DM: 1;
                bool BP: 1;
                bool OF: 1;
                bool NF: 1;
            };
            char flags{0};
        };
//Regs:
        uint8_t A{0}, X{0}, Y{0}, S{0};
        uint16_t PC{0};
        uint8_t P{0};
        uint8_t RAM[1 << 13];
//Addressing Modes:

        uint8_t immediate();

        uint8_t zero_page();

        uint8_t zero_page_x();

        uint8_t zero_page_y();

        uint8_t relative();

        uint8_t absolute();

        uint8_t absolute_x();

        uint8_t absolute_y();

        uint8_t indirect();

        uint8_t indexed_indirect();

        uint8_t inderect_indexed();

    };
}
#endif //R6502_H
