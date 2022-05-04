#pragma once

#include <cstdint>
#include <array>
#include <variant>
#include "bus.h"
#include "connect_token.h"
#include <memory>
#include "nes_exceptions.h"
#include "nes_devices/dma.h"
#include "reset_token.h"

namespace NES {
class Cpu;
using no_param = void (Cpu::*)();

#pragma pack(push, 1)

struct Instruction {
    no_param func;
    no_param addr_mod;
};

struct CpuData {
    union {
        struct {
            bool CF     : 1;
            bool ZF     : 1;
            bool ID     : 1;
            bool DM     : 1;
            bool BC     : 1;
            bool UNUSED : 1;
            bool OF     : 1;
            bool NF     : 1;
        };
        uint8_t flags;
    };

    // Regs:

    uint8_t A, X, Y, SP;
    uint16_t PC;

    // DMA

    bool dma_is_active = false;

    // Additional variables

    bool accumulator_mod{false};
    uint16_t last_absolute_address;
    uint16_t last_relative_address;
    uint8_t cycles;
};

class Cpu : CpuData {
public:
    Cpu();
    void connect(Bus *bus_, ConnectToken token) noexcept;
    void tick(bool even_cycle);
    void save(std::ofstream &file);
    void load(std::ifstream &file);
    // interrupts:
    void reset(ResetToken);
    void NMI();
    void IRQ();

private:
    // Bus

    Bus *bus;

    // DMA

    Dma dma;

    // Methods for filling the array:

    void throw_exception();

    // Addressing Modes:

    void implicit();  void immediate(); void zero_page_y(); void absolute_x(); void accumulator();
    void zero_page(); void relative(); void absolute_y(); void indirect_indexed();
    void zero_page_x(); void absolute(); void indirect(); void indexed_indirect();

    // Instructions:

    void ADC(); void BCS(); void BNE(); void BVS(); void SEC(); void LDA(); void ORA();
    void AND(); void BEQ(); void BPL(); void CMP(); void CLC(); void EOR(); void TSX();
    void ASL(); void BIT(); void SBC(); void CPX(); void CLD(); void TXS(); void TYA();
    void BCC(); void BMI(); void BVC(); void CPY(); void CLI(); void STY(); void TAX();
    void LSR(); void CLV(); void DEC(); void INC(); void JMP(); void PHA(); void LDX();
    void ROR(); void DEX(); void INX(); void JSR(); void PHP(); void BRK(); void TXA();
    void LDY(); void DEY(); void INY(); void NOP(); void PLA(); void SEI(); void STX();
    void ROL(); void SED(); void RTI(); void PLP(); void RTS(); void STA(); void TAY();
    std::array<Instruction, 1 << 8> map_opcodes;
    std::array<uint8_t, 1 << 8> map_cycles;
    // Others
    uint8_t cpu_read(uint16_t addr);
    void cpu_write(uint16_t addr, uint8_t data);
    void add_relative();
    void cmp_with(uint8_t T);
    void push_on_stack(uint8_t T);
};

#pragma pack(pop)

}  // namespace NES
