#ifndef R6502_H
#define R6502_H
#include <cstdint>
#include <array>
#include <variant>
#include<stdexcept>
namespace NES {
    struct IncorrectOpcode: std::runtime_error{
        IncorrectOpcode();
    };
    class CPU;
    struct I {
        //I(std::variant<void(CPU::*)() ,void(CPU::*)(uint8_t)> func_,  uint8_t(CPU::*addr_mod_)());
        std::variant<void(CPU::*)() ,void(CPU::*)(uint8_t)> func;
        uint8_t(CPU::*addr_mod)();
    };

    class CPU {
    public:
        CPU();
    private:
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
//RAM/SRAM:

        std::array<uint8_t, 1<<13> RAM{};
        std::array<uint8_t, 1<<13> SRAM{};

//Additional variables:

        uint16_t last_address{};

//Addressing Modes:

        uint8_t implicit();  uint8_t immediate(); uint8_t zero_page_y(); uint8_t absolute_x(); uint8_t accumulator();
        uint8_t zero_page(); uint8_t relative(); uint8_t absolute_y(); uint8_t indirect_indexed();
        uint8_t zero_page_x(); uint8_t absolute(); uint8_t indirect();uint8_t indexed_indirect();

//Special:

        void throw_exception();
//Instructions:
    void ADC(uint8_t data); void BCS(uint8_t data); void BNE(uint8_t data); void BVS(uint8_t data);void SEC(); void LDA(uint8_t data); void LSR(uint8_t data); void CLV(); void DEC(); void INC(); void JMP(); void PHA(); void STX(); void TSX();
    void AND(uint8_t data); void BEQ(uint8_t data); void BPL(uint8_t data); void CMP(uint8_t data);void CLC(); void ORA(uint8_t data); void ROR(uint8_t data); void DEX(); void INX(); void JSR(); void PHP(); void BRK(); void STY(); void TXA();
    void ASL(uint8_t data); void BIT(uint8_t data); void SBC(uint8_t data); void CPX(uint8_t data);void CLD(); void LDX(uint8_t data); void LDY(uint8_t data); void DEY(); void INY(); void NOP(); void PLA(); void SEI(); void TAX(); void TXS();
    void BCC(uint8_t data); void BMI(uint8_t data); void BVC(uint8_t data); void CPY(uint8_t data);void CLI(); void EOR(uint8_t data); void ROL(uint8_t data); void SED(); void RTI(); void PLP(); void RTS(); void STA(); void TAY(); void TYA();
    std::array<I,1<<8> map_opcodes;
    };

}
#endif //R6502_H
