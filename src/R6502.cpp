#include "R6502.h"
#include <utility>
namespace NES {

void CPU::push_on_stack(uint8_t T) {
    (*bus).mem_write(SP + 0x0100, T);
    SP--;
}

CPU::CPU()
    : map_opcodes{I{&CPU::BRK, &CPU::implicit},
                  I{&CPU::ORA, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ORA, &CPU::zero_page},
                  I{&CPU::ASL, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::PHP, &CPU::implicit},
                  I{&CPU::ORA, &CPU::immediate},
                  I{&CPU::ASL, &CPU::accumulator},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ORA, &CPU::absolute},
                  I{&CPU::ASL, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BPL, &CPU::relative},
                  I{&CPU::ORA, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ORA, &CPU::zero_page_x},
                  I{&CPU::ASL, &CPU::zero_page_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CLC, &CPU::implicit},
                  I{&CPU::ORA, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ORA, &CPU::absolute_x},
                  I{&CPU::ASL, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::JSR, &CPU::absolute},
                  I{&CPU::AND, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BIT, &CPU::zero_page},
                  I{&CPU::AND, &CPU::zero_page},
                  I{&CPU::ROL, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::PLP, &CPU::implicit},
                  I{&CPU::AND, &CPU::immediate},
                  I{&CPU::ROL, &CPU::accumulator},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BIT, &CPU::absolute},
                  I{&CPU::AND, &CPU::absolute},
                  I{&CPU::ROL, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BMI, &CPU::relative},
                  I{&CPU::AND, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::AND, &CPU::zero_page_x},
                  I{&CPU::ROL, &CPU::zero_page_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::SEC, &CPU::implicit},
                  I{&CPU::AND, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::AND, &CPU::absolute_x},
                  I{&CPU::ROL, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::RTI, &CPU::implicit},
                  I{&CPU::EOR, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::EOR, &CPU::zero_page},
                  I{&CPU::LSR, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::PHA, &CPU::implicit},
                  I{&CPU::EOR, &CPU::immediate},
                  I{&CPU::LSR, &CPU::accumulator},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::JMP, &CPU::absolute},
                  I{&CPU::EOR, &CPU::absolute},
                  I{&CPU::LSR, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BVC, &CPU::relative},
                  I{&CPU::EOR, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::EOR, &CPU::zero_page_x},
                  I{&CPU::LSR, &CPU::zero_page_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CLI, &CPU::implicit},
                  I{&CPU::EOR, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::EOR, &CPU::absolute_x},
                  I{&CPU::LSR, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::RTS, &CPU::implicit},
                  I{&CPU::ADC, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ADC, &CPU::zero_page},
                  I{&CPU::ROR, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::PLA, &CPU::implicit},
                  I{&CPU::ADC, &CPU::immediate},
                  I{&CPU::ROR, &CPU::accumulator},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::JMP, &CPU::indirect},
                  I{&CPU::ADC, &CPU::absolute},
                  I{&CPU::ROR, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BVS, &CPU::relative},
                  I{&CPU::ADC, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ADC, &CPU::zero_page_x},
                  I{&CPU::ROR, &CPU::zero_page_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::SEI, &CPU::implicit},
                  I{&CPU::ADC, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::ADC, &CPU::absolute_x},
                  I{&CPU::ROR, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::STA, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::STY, &CPU::zero_page},
                  I{&CPU::STA, &CPU::zero_page},
                  I{&CPU::STX, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::DEY, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::TXA, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::STY, &CPU::absolute},
                  I{&CPU::STA, &CPU::absolute},
                  I{&CPU::STX, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BCC, &CPU::relative},
                  I{&CPU::STA, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::STY, &CPU::zero_page_x},
                  I{&CPU::STA, &CPU::zero_page_x},
                  I{&CPU::STX, &CPU::zero_page_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::TYA, &CPU::implicit},
                  I{&CPU::STA, &CPU::absolute_y},
                  I{&CPU::TXS, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::STA, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::LDY, &CPU::immediate},
                  I{&CPU::LDA, &CPU::indexed_indirect},
                  I{&CPU::LDX, &CPU::immediate},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::LDY, &CPU::zero_page},
                  I{&CPU::LDA, &CPU::zero_page},
                  I{&CPU::LDX, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::TAY, &CPU::implicit},
                  I{&CPU::LDA, &CPU::immediate},
                  I{&CPU::TAX, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::LDY, &CPU::absolute},
                  I{&CPU::LDA, &CPU::absolute},
                  I{&CPU::LDX, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BCS, &CPU::relative},
                  I{&CPU::LDA, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::LDY, &CPU::zero_page_x},
                  I{&CPU::LDA, &CPU::zero_page_x},
                  I{&CPU::LDX, &CPU::zero_page_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CLV, &CPU::implicit},
                  I{&CPU::LDA, &CPU::absolute_y},
                  I{&CPU::TSX, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::LDY, &CPU::absolute_x},
                  I{&CPU::LDA, &CPU::absolute_x},
                  I{&CPU::LDX, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CPY, &CPU::immediate},
                  I{&CPU::CMP, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CPY, &CPU::zero_page},
                  I{&CPU::CMP, &CPU::zero_page},
                  I{&CPU::DEC, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::INY, &CPU::implicit},
                  I{&CPU::CMP, &CPU::immediate},
                  I{&CPU::DEX, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CPY, &CPU::absolute},
                  I{&CPU::CMP, &CPU::absolute},
                  I{&CPU::DEC, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BNE, &CPU::relative},
                  I{&CPU::CMP, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CMP, &CPU::zero_page_x},
                  I{&CPU::DEC, &CPU::zero_page_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CLD, &CPU::implicit},
                  I{&CPU::CMP, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CMP, &CPU::absolute_x},
                  I{&CPU::DEC, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CPX, &CPU::immediate},
                  I{&CPU::SBC, &CPU::indexed_indirect},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CPX, &CPU::zero_page},
                  I{&CPU::SBC, &CPU::zero_page},
                  I{&CPU::INC, &CPU::zero_page},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::INX, &CPU::implicit},
                  I{&CPU::SBC, &CPU::immediate},
                  I{&CPU::NOP, &CPU::implicit},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::CPX, &CPU::absolute},
                  I{&CPU::SBC, &CPU::absolute},
                  I{&CPU::INC, &CPU::absolute},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::BEQ, &CPU::relative},
                  I{&CPU::SBC, &CPU::indirect_indexed},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::SBC, &CPU::zero_page_x},
                  I{&CPU::INC, &CPU::zero_page_x},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::SED, &CPU::implicit},
                  I{&CPU::SBC, &CPU::absolute_y},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::throw_exception, &CPU::throw_exception},
                  I{&CPU::SBC, &CPU::absolute_x},
                  I{&CPU::INC, &CPU::absolute_x},
                  I{&CPU::throw_exception, &CPU::throw_exception}},
      map_cycles{7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0, 2, 2, 0, 0, 0, 4, 6, 0, 2, 4,
                 0, 0, 0, 4, 7, 0, 6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0, 2, 5, 0, 0,
                 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, 6, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 3, 4,
                 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, 6, 6, 0, 0, 0, 3, 5, 0,
                 4, 2, 2, 0, 5, 4, 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, 0, 6,
                 0, 0, 3, 3, 3, 0, 2, 0, 2, 0, 4, 4, 4, 0, 2, 6, 0, 0, 4, 4, 4, 0, 2, 5, 2, 0,
                 0, 5, 0, 0, 2, 6, 2, 0, 3, 3, 3, 0, 2, 2, 2, 0, 4, 4, 4, 0, 2, 5, 0, 0, 4, 4,
                 4, 0, 2, 4, 2, 0, 4, 4, 4, 0, 2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0,
                 0, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0, 2, 6, 0, 0, 3, 3, 5, 0, 2, 2,
                 2, 0, 4, 4, 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0} {}

IncorrectOpcode::IncorrectOpcode() : std::runtime_error("Incorrect opcode!") {}

void CPU::connect(Bus *bus_, ConnectToken) noexcept {
    bus = bus_;
}

void CPU::implicit() {}  // just do nothing

void CPU::immediate() {
    last_absolute_address = PC++;
}

void CPU::zero_page() {
    last_absolute_address = (*bus).mem_read(PC++);
    last_absolute_address &= 0x00FF;
}

void CPU::zero_page_x() {
    last_absolute_address = (*bus).mem_read(PC++);
    last_absolute_address += X;
    last_absolute_address &= 0x00FF;
}

void CPU::zero_page_y() {
    last_absolute_address = (*bus).mem_read(PC++);
    last_absolute_address += Y;
    last_absolute_address &= 0x00FF;
}

void CPU::relative() {
    last_relative_address = (*bus).mem_read(PC++);
    if (last_relative_address >> 8) {
        last_relative_address += 0xFF00;
    }
}

void CPU::absolute() {
    last_absolute_address = (*bus).mem_read(PC++);
    last_absolute_address <<= 8;
    last_absolute_address |= (*bus).mem_read(PC++);
}

void CPU::absolute_x() {
    absolute();
    last_absolute_address += X;
    if ((last_absolute_address >> 8) != ((last_absolute_address - X) >> 8))
        cycles++;
}

void CPU::absolute_y() {
    absolute();
    last_absolute_address += Y;
    if ((last_absolute_address >> 8) != ((last_absolute_address - Y) >> 8))
        cycles++;
}

void CPU::indirect() {
    uint8_t LSB_temp = (*bus).mem_read(PC++);
    uint8_t MSB_temp = (*bus).mem_read(PC++);
    uint16_t temp_address = MSB_temp;
    temp_address <<= 8;
    temp_address += LSB_temp;
    uint8_t LSB = (*bus).mem_read(temp_address);
    if ((LSB_temp & 0x00FF) == 0x00FF) {
        last_absolute_address = ((*bus).mem_read(temp_address & 0xFF00));  // bug
    } else {
        last_absolute_address = ((*bus).mem_read(++temp_address));
    }
    last_absolute_address <<= 8;
    last_absolute_address += LSB;
}

void CPU::indexed_indirect() {
    uint16_t temp_address = (*bus).mem_read(PC++);
    uint8_t LSB = (*bus).mem_read((temp_address + X) & 0x00FF);
    last_absolute_address = (*bus).mem_read((temp_address + X + 1) & 0x00FF);
    last_absolute_address <<= 8;
    last_absolute_address += LSB;
}

void CPU::indirect_indexed() {
    uint16_t temp_address = (*bus).mem_read(PC++);
    uint8_t LSB = (*bus).mem_read((temp_address)&0x00FF);
    last_absolute_address = (*bus).mem_read((temp_address + 1) & 0x00FF);
    last_absolute_address <<= 8;
    last_absolute_address += LSB;
    last_absolute_address += Y;
    if ((last_absolute_address >> 8) != ((last_absolute_address - Y) >> 8))
        cycles++;
}

void CPU::accumulator() {
    accumulator_mod = true;
}

void CPU::ADC() {
    uint16_t temp = (*bus).mem_read(last_absolute_address);
    CF = (temp + A + CF) >> 8;
    OF = (temp & A & 0x80 & (~((temp + A + CF) & 0x80)) ||
          (~(temp & 0x80)) & (~(A & 0x80)) & ((temp + A + CF) & 0x80));
    bool CF_temp = (temp + A + CF) >> 8;
    A = (temp + A + CF) & 0x00FF;
    CF = CF_temp;
    ZF = !A;
}

void CPU::AND() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    A &= temp;
    ZF = (!A);
    NF = (A >> 7);
}

void CPU::ASL() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = (*bus).mem_read(last_absolute_address);
    temp <<= 1;
    CF = (temp >> 8);
    ZF = !(temp & 0x00FF);
    NF = (temp & 128);
    if (!accumulator_mod) {
        (*bus).mem_write(last_absolute_address, temp & 0x00FF);
    } else {
        A = temp & 0x00FF;
    }
}

void CPU::add_relative() {
    uint16_t temp = PC + last_relative_address;
    cycles++;
    if ((temp >> 8) != (PC >> 8)) {
        cycles++;
    }
    PC = temp;
}

void CPU::BCC() {
    if (!CF) {
        add_relative();
    }
}

void CPU::BCS() {
    if (CF) {
        add_relative();
    }
}

void CPU::BEQ() {
    if (ZF) {
        add_relative();
    }
}

void CPU::BIT() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    temp &= A;
    ZF = !temp;
    NF = temp & 0x80;
    OF = temp & 0x80;
}

void CPU::BMI() {
    if (NF) {
        add_relative();
    }
}

void CPU::BNE() {
    if (!ZF) {
        add_relative();
    }
}

void CPU::BPL() {
    if (!NF) {
        add_relative();
    }
}

void CPU::BRK() {
    BC = true;
    push_on_stack(PC >> 8);
    push_on_stack(PC & 0xFF);
    push_on_stack(flags);
    BC = false;
    ID = true;
    uint16_t temp = (*bus).mem_read(0xFFFE);
    PC = temp;
    temp = (*bus).mem_read(0xFFFF);
    PC |= temp << 8;
}

void CPU::BVC() {
    if (!OF) {
        add_relative();
    }
}

void CPU::BVS() {
    if (OF) {
        add_relative();
    }
}

void CPU::CLC() {
    CF = false;
}

void CPU::CLD() {
    DM = false;
}

void CPU::CLI() {
    ID = false;
}

void CPU::CLV() {
    OF = false;
}

void CPU::cmp_with(uint8_t T) {
    uint16_t read = (*bus).mem_read(last_absolute_address);
    uint16_t temp = T - read;
    ZF = !temp;
    NF = temp & 0x80;
    CF = T >= read;
}

void CPU::CMP() {
    cmp_with(A);
}

void CPU::CPX() {
    cmp_with(X);
}

void CPU::CPY() {
    cmp_with(Y);
}

void CPU::DEC() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    temp--;
    ZF = !temp;
    NF = temp & 0x80;
    (*bus).mem_write(last_absolute_address, temp & 0x00FF);
}

void CPU::DEX() {
    X--;
    ZF = !X;
    NF = X & 0x80;
}

void CPU::DEY() {
    Y--;
    ZF = !Y;
    NF = Y & 0x80;
}

void CPU::EOR() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    A ^= temp;
    ZF = !A;
    NF = A & 0x80;
}

void CPU::INC() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    temp++;
    ZF = !temp;
    NF = temp & 0x80;
    (*bus).mem_write(last_absolute_address, temp & 0x00FF);
}

void CPU::INX() {
    X++;
    ZF = !X;
    NF = X & 0x80;
}

void CPU::INY() {
    Y--;
    ZF = !Y;
    NF = Y & 0x80;
}

void CPU::JMP() {
    uint16_t temp = (*bus).mem_read(last_absolute_address);
    PC = temp;
}

void CPU::JSR() {
    PC--;
    push_on_stack(PC >> 8);
    push_on_stack(PC & 0xFF);
    PC = last_absolute_address;
}

void CPU::LDA() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    A = temp;
}

void CPU::LDX() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    X = temp;
}

void CPU::LDY() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    Y = temp;
}

void CPU::LSR() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = (*bus).mem_read(last_absolute_address);
    CF = temp & 1;
    temp >>= 1;
    ZF = !(temp & 0x00FF);
    NF = (temp & 128);
    if (!accumulator_mod) {
        (*bus).mem_write(last_absolute_address, temp & 0x00FF);
    } else {
        A = temp & 0x00FF;
    }
}

void CPU::NOP() {}

void CPU::ORA() {
    uint8_t temp = (*bus).mem_read(last_absolute_address);
    A |= temp;
    ZF = !A;
    NF = A & 0x80;
}

void CPU::PHA() {
    push_on_stack(A);
}

void CPU::PHP() {
    push_on_stack(flags);
}

void CPU::PLA() {
    A = (*bus).mem_read(++SP + 0x0100);
    ZF = !A;
    NF = A & 0x80;
}

void CPU::PLP() {
    flags = (*bus).mem_read(++SP + 0x0100);
}

void CPU::ROL() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = (*bus).mem_read(last_absolute_address);
    temp <<= 1;
    temp += CF;
    CF = temp & 0x100;
    NF = temp & 0x80;
    if (!accumulator_mod) {
        (*bus).mem_write(last_absolute_address, temp & 0x00FF);
    } else {
        A = temp & 0x00FF;
        ZF = !A;
    }
}

void CPU::ROR() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = (*bus).mem_read(last_absolute_address);
    temp += 0x100 * static_cast<uint16_t>(CF);
    CF = temp & 1;
    temp >>= 1;
    NF = temp & 0x80;
    if (!accumulator_mod) {
        (*bus).mem_write(last_absolute_address, temp & 0x00FF);
    } else {
        A = temp & 0x00FF;
        ZF = !A;
    }
}

void CPU::RTI() {
    SP++;
    flags = (*bus).mem_read(SP + 0x0100);
    SP++;
    uint16_t temp = (*bus).mem_read(SP + 0x0100);
    PC = temp;
    SP++;
    temp = (*bus).mem_read(SP + 0x0100);
    PC |= temp << 8;
}

void CPU::RTS() {
    SP++;
    uint16_t temp = (*bus).mem_read(SP + 0x0100);
    PC = temp;
    SP++;
    temp = (*bus).mem_read(SP + 0x0100);
    PC |= temp << 8;
}

void CPU::SBC() {
    uint16_t temp = (*bus).mem_read(last_absolute_address);
    OF = (temp & A & 0x80 & (~((A - temp - (1 - CF)) & 0x80)) ||
          (~(temp & 0x80)) & (~(A & 0x80)) & ((A - temp - (1 - CF)) & 0x80));
    bool CF_temp = (A - temp - (1 - CF)) >> 8;
    A = (A - temp - (1 - CF)) & 0x00FF;
    CF = CF_temp;
    ZF = !A;
}

void CPU::SEC() {
    CF = true;
}

void CPU::SED() {
    DM = true;
}

void CPU::SEI() {
    ID = true;
}

void CPU::STA() {
    (*bus).mem_write(last_absolute_address, A);
}

void CPU::STX() {
    (*bus).mem_write(last_absolute_address, X);
}

void CPU::STY() {
    (*bus).mem_write(last_absolute_address, Y);
}

void CPU::TAX() {
    X = A;
    ZF = !X;
    NF = X & 0x80;
}

void CPU::TAY() {
    Y = A;
    ZF = !Y;
    NF = Y & 0x80;
}

void CPU::TSX() {
    X = SP;
    ZF = !X;
    NF = X & 0x80;
}

void CPU::TXA() {
    A = X;
    ZF = !A;
    NF = A & 0x80;
}

void CPU::TXS() {
    SP = X;
}

void CPU::TYA() {
    A = Y;
    ZF = !A;
    NF = A & 0x80;
}

void CPU::throw_exception() {
    throw IncorrectOpcode();
}

void CPU::clock() {
    if (cycles) {
        cycles--;
        return;
    }
    uint8_t current_opcode = (*bus).mem_read(PC++);
    I current_instruction = map_opcodes[current_opcode];
    cycles = map_cycles[current_opcode];
    (this->*current_instruction.addr_mod)();
    (this->*current_instruction.func)();
}

void CPU::interrupt() {
    push_on_stack(PC >> 8);
    push_on_stack(PC & 0xFF);
    BC = false;
    ID = true;
    push_on_stack(flags);
    uint16_t temp = (*bus).mem_read(0xFFFA);
    PC = temp;
    temp = (*bus).mem_read(0xFFFB);
    PC |= temp << 8;
    cycles = 7;
}

void CPU::reset() {
    A = 0;
    X = 0;
    Y = 0;
    flags = 0;
    SP = 0xFD;  // The Stack is set at $FD, with two values already stacked ($00, $00).
    last_absolute_address = 0;
    last_relative_address = 0;
    accumulator_mod = false;
    uint16_t temp = (*bus).mem_read(0xFFFC);
    PC = temp;
    temp = (*bus).mem_read(0xFFFD);
    PC |= temp << 8;
    cycles = 8;
}

void CPU::NMI() {
    interrupt();
}

void CPU::IRQ() {
    if (!ID) {
        interrupt();
    }
}

}  // namespace NES
