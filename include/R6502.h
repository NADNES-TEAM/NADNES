#ifndef R6502_H
#define R6502_H
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
            char flags;
        }

    };
}
#endif //R6502_H
