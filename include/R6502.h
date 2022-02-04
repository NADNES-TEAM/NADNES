#ifndef R6502_H
#define R6502_H
#include <cstdint>

namespace NES {
    class  CPU {
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
        uint8_t A{0}, X{0}, Y{0}, S{0};
        uint16_t PC{0};
        uint8_t P{0};
    };
}
#endif //R6502_H
