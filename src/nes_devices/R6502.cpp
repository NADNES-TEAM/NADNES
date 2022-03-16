#include "nes_devices/R6502.h"
#include <iostream>
#include <utility>

namespace NES {

uint8_t Cpu::Cpu_read(uint16_t addr) {
    return (*bus).mem_read(addr);
}

void Cpu::Cpu_write(uint16_t addr, uint8_t data) {
    (*bus).mem_write(addr, data);
}
void Cpu::push_on_stack(uint8_t T) {
    Cpu_write(SP + 0x0100, T);
    SP--;
}

Cpu::Cpu()
    : map_opcodes{Instruction{&Cpu::BRK, &Cpu::implicit},
                  Instruction{&Cpu::ORA, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ORA, &Cpu::zero_page},
                  Instruction{&Cpu::ASL, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::PHP, &Cpu::implicit},
                  Instruction{&Cpu::ORA, &Cpu::immediate},
                  Instruction{&Cpu::ASL, &Cpu::accumulator},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ORA, &Cpu::absolute},
                  Instruction{&Cpu::ASL, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BPL, &Cpu::relative},
                  Instruction{&Cpu::ORA, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ORA, &Cpu::zero_page_x},
                  Instruction{&Cpu::ASL, &Cpu::zero_page_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CLC, &Cpu::implicit},
                  Instruction{&Cpu::ORA, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ORA, &Cpu::absolute_x},
                  Instruction{&Cpu::ASL, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::JSR, &Cpu::absolute},
                  Instruction{&Cpu::AND, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BIT, &Cpu::zero_page},
                  Instruction{&Cpu::AND, &Cpu::zero_page},
                  Instruction{&Cpu::ROL, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::PLP, &Cpu::implicit},
                  Instruction{&Cpu::AND, &Cpu::immediate},
                  Instruction{&Cpu::ROL, &Cpu::accumulator},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BIT, &Cpu::absolute},
                  Instruction{&Cpu::AND, &Cpu::absolute},
                  Instruction{&Cpu::ROL, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BMI, &Cpu::relative},
                  Instruction{&Cpu::AND, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::AND, &Cpu::zero_page_x},
                  Instruction{&Cpu::ROL, &Cpu::zero_page_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::SEC, &Cpu::implicit},
                  Instruction{&Cpu::AND, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::AND, &Cpu::absolute_x},
                  Instruction{&Cpu::ROL, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::RTI, &Cpu::implicit},
                  Instruction{&Cpu::EOR, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::EOR, &Cpu::zero_page},
                  Instruction{&Cpu::LSR, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::PHA, &Cpu::implicit},
                  Instruction{&Cpu::EOR, &Cpu::immediate},
                  Instruction{&Cpu::LSR, &Cpu::accumulator},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::JMP, &Cpu::absolute},
                  Instruction{&Cpu::EOR, &Cpu::absolute},
                  Instruction{&Cpu::LSR, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BVC, &Cpu::relative},
                  Instruction{&Cpu::EOR, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::EOR, &Cpu::zero_page_x},
                  Instruction{&Cpu::LSR, &Cpu::zero_page_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CLI, &Cpu::implicit},
                  Instruction{&Cpu::EOR, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::EOR, &Cpu::absolute_x},
                  Instruction{&Cpu::LSR, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::RTS, &Cpu::implicit},
                  Instruction{&Cpu::ADC, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ADC, &Cpu::zero_page},
                  Instruction{&Cpu::ROR, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::PLA, &Cpu::implicit},
                  Instruction{&Cpu::ADC, &Cpu::immediate},
                  Instruction{&Cpu::ROR, &Cpu::accumulator},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::JMP, &Cpu::indirect},
                  Instruction{&Cpu::ADC, &Cpu::absolute},
                  Instruction{&Cpu::ROR, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BVS, &Cpu::relative},
                  Instruction{&Cpu::ADC, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ADC, &Cpu::zero_page_x},
                  Instruction{&Cpu::ROR, &Cpu::zero_page_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::SEI, &Cpu::implicit},
                  Instruction{&Cpu::ADC, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::ADC, &Cpu::absolute_x},
                  Instruction{&Cpu::ROR, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::STA, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::STY, &Cpu::zero_page},
                  Instruction{&Cpu::STA, &Cpu::zero_page},
                  Instruction{&Cpu::STX, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::DEY, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::TXA, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::STY, &Cpu::absolute},
                  Instruction{&Cpu::STA, &Cpu::absolute},
                  Instruction{&Cpu::STX, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BCC, &Cpu::relative},
                  Instruction{&Cpu::STA, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::STY, &Cpu::zero_page_x},
                  Instruction{&Cpu::STA, &Cpu::zero_page_x},
                  Instruction{&Cpu::STX, &Cpu::zero_page_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::TYA, &Cpu::implicit},
                  Instruction{&Cpu::STA, &Cpu::absolute_y},
                  Instruction{&Cpu::TXS, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::STA, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::LDY, &Cpu::immediate},
                  Instruction{&Cpu::LDA, &Cpu::indexed_indirect},
                  Instruction{&Cpu::LDX, &Cpu::immediate},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::LDY, &Cpu::zero_page},
                  Instruction{&Cpu::LDA, &Cpu::zero_page},
                  Instruction{&Cpu::LDX, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::TAY, &Cpu::implicit},
                  Instruction{&Cpu::LDA, &Cpu::immediate},
                  Instruction{&Cpu::TAX, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::LDY, &Cpu::absolute},
                  Instruction{&Cpu::LDA, &Cpu::absolute},
                  Instruction{&Cpu::LDX, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BCS, &Cpu::relative},
                  Instruction{&Cpu::LDA, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::LDY, &Cpu::zero_page_x},
                  Instruction{&Cpu::LDA, &Cpu::zero_page_x},
                  Instruction{&Cpu::LDX, &Cpu::zero_page_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CLV, &Cpu::implicit},
                  Instruction{&Cpu::LDA, &Cpu::absolute_y},
                  Instruction{&Cpu::TSX, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::LDY, &Cpu::absolute_x},
                  Instruction{&Cpu::LDA, &Cpu::absolute_x},
                  Instruction{&Cpu::LDX, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CPY, &Cpu::immediate},
                  Instruction{&Cpu::CMP, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CPY, &Cpu::zero_page},
                  Instruction{&Cpu::CMP, &Cpu::zero_page},
                  Instruction{&Cpu::DEC, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::INY, &Cpu::implicit},
                  Instruction{&Cpu::CMP, &Cpu::immediate},
                  Instruction{&Cpu::DEX, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CPY, &Cpu::absolute},
                  Instruction{&Cpu::CMP, &Cpu::absolute},
                  Instruction{&Cpu::DEC, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BNE, &Cpu::relative},
                  Instruction{&Cpu::CMP, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CMP, &Cpu::zero_page_x},
                  Instruction{&Cpu::DEC, &Cpu::zero_page_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CLD, &Cpu::implicit},
                  Instruction{&Cpu::CMP, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CMP, &Cpu::absolute_x},
                  Instruction{&Cpu::DEC, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CPX, &Cpu::immediate},
                  Instruction{&Cpu::SBC, &Cpu::indexed_indirect},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CPX, &Cpu::zero_page},
                  Instruction{&Cpu::SBC, &Cpu::zero_page},
                  Instruction{&Cpu::INC, &Cpu::zero_page},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::INX, &Cpu::implicit},
                  Instruction{&Cpu::SBC, &Cpu::immediate},
                  Instruction{&Cpu::NOP, &Cpu::implicit},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::CPX, &Cpu::absolute},
                  Instruction{&Cpu::SBC, &Cpu::absolute},
                  Instruction{&Cpu::INC, &Cpu::absolute},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::BEQ, &Cpu::relative},
                  Instruction{&Cpu::SBC, &Cpu::indirect_indexed},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::SBC, &Cpu::zero_page_x},
                  Instruction{&Cpu::INC, &Cpu::zero_page_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::SED, &Cpu::implicit},
                  Instruction{&Cpu::SBC, &Cpu::absolute_y},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception},
                  Instruction{&Cpu::SBC, &Cpu::absolute_x},
                  Instruction{&Cpu::INC, &Cpu::absolute_x},
                  Instruction{&Cpu::throw_exception, &Cpu::throw_exception}},
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

void Cpu::connect(Bus *bus_, ConnectToken) noexcept {
    bus = bus_;
}

void Cpu::implicit() {}  // just do nothing

void Cpu::immediate() {
    last_absolute_address = PC++;
}

void Cpu::zero_page() {
    last_absolute_address = Cpu_read(PC++);
    last_absolute_address &= 0xFF;
}

void Cpu::zero_page_x() {
    last_absolute_address = Cpu_read(PC++);
    last_absolute_address += X;
    last_absolute_address &= 0xFF;
}

void Cpu::zero_page_y() {
    last_absolute_address = Cpu_read(PC++);
    last_absolute_address += Y;
    last_absolute_address &= 0xFF;
}

void Cpu::relative() {
    last_relative_address = Cpu_read(PC++);
    if (last_relative_address & 0x80) {
        last_relative_address |= 0xFF00;
    }
}

void Cpu::absolute() {
    uint16_t temp = Cpu_read(PC++);
    last_absolute_address = Cpu_read(PC++);
    last_absolute_address <<= 8;
    last_absolute_address |= temp;
}

void Cpu::absolute_x() {
    absolute();
    last_absolute_address += X;
    if ((last_absolute_address >> 8) != ((last_absolute_address - X) >> 8))
        cycles++;
}

void Cpu::absolute_y() {
    absolute();
    last_absolute_address += Y;
    if ((last_absolute_address >> 8) != ((last_absolute_address - Y) >> 8))
        cycles++;
}

void Cpu::indirect() {
    uint8_t LSB_temp = Cpu_read(PC++);
    uint8_t MSB_temp = Cpu_read(PC++);
    uint16_t temp_address = MSB_temp;
    temp_address <<= 8;
    temp_address |= LSB_temp;
    uint16_t LSB_final = Cpu_read(temp_address);
    if ((LSB_temp & 0xFF) == 0xFF) {
        last_absolute_address = (Cpu_read(temp_address & 0xFF00));  // bug
    } else {
        last_absolute_address = (Cpu_read(temp_address + 1));
    }
    last_absolute_address <<= 8;
    last_absolute_address |= LSB_final;
}

void Cpu::indexed_indirect() {
    uint16_t temp_address = Cpu_read(PC++);
    uint8_t LSB = Cpu_read((temp_address + X) & 0xFF);
    last_absolute_address = Cpu_read((temp_address + X + 1) & 0xFF);
    last_absolute_address <<= 8;
    last_absolute_address |= LSB;
}

void Cpu::indirect_indexed() {
    uint16_t temp_address = Cpu_read(PC++);
    uint8_t LSB = Cpu_read((temp_address)&0xFF);
    last_absolute_address = Cpu_read((temp_address + 1) & 0xFF);
    last_absolute_address <<= 8;
    last_absolute_address |= LSB;
    last_absolute_address += Y;
    if ((last_absolute_address >> 8) != ((last_absolute_address - Y) >> 8))
        cycles++;
}

void Cpu::accumulator() {
    accumulator_mod = true;
}

void Cpu::ADC() {
    uint16_t temp = Cpu_read(last_absolute_address);
    uint16_t temp_A = temp + A + CF;
    CF = (temp_A) >> 8;
    OF = ((temp_A ^ A) & 0x80 && (temp_A ^ temp) & 0x80);
    A = (temp_A)&0xFF;
    ZF = !A;
    NF = temp_A & 0x80;
}

void Cpu::AND() {
    uint8_t temp = Cpu_read(last_absolute_address);
    A &= temp;
    ZF = (!A);
    NF = A & 0x80;
}

void Cpu::ASL() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = Cpu_read(last_absolute_address);
    temp <<= 1;
    CF = (temp >> 8);
    ZF = !(temp & 0xFF);
    NF = (temp & 128);
    if (!accumulator_mod) {
        Cpu_write(last_absolute_address, temp & 0xFF);
    } else {
        A = temp & 0xFF;
    }
}

void Cpu::add_relative() {
    uint16_t temp = PC + last_relative_address;
    cycles++;
    if ((temp >> 8) != (PC >> 8)) {
        cycles++;
    }
    PC = temp;
}

void Cpu::BCC() {
    if (!CF) {
        add_relative();
    }
}

void Cpu::BCS() {
    if (CF) {
        add_relative();
    }
}

void Cpu::BEQ() {
    if (ZF) {
        add_relative();
    }
}

void Cpu::BIT() {
    uint8_t temp = Cpu_read(last_absolute_address);
    ZF = !(temp & A);
    NF = temp & 0x80;
    OF = temp & 0x40;
}

void Cpu::BMI() {
    if (NF) {
        add_relative();
    }
}

void Cpu::BNE() {
    if (!ZF) {
        add_relative();
    }
}

void Cpu::BPL() {
    if (!NF) {
        add_relative();
    }
}

void Cpu::BRK() {
    BC = true;
    push_on_stack(PC >> 8);
    push_on_stack(PC & 0xFF);
    push_on_stack(flags);
    BC = false;
    ID = true;
    uint16_t temp = Cpu_read(0xFFFE);
    PC = temp;
    temp = Cpu_read(0xFFFF);
    PC |= temp << 8;
}

void Cpu::BVC() {
    if (!OF) {
        add_relative();
    }
}

void Cpu::BVS() {
    if (OF) {
        add_relative();
    }
}

void Cpu::CLC() {
    CF = false;
}

void Cpu::CLD() {
    DM = false;
}

void Cpu::CLI() {
    ID = false;
}

void Cpu::CLV() {
    OF = false;
}

void Cpu::cmp_with(uint8_t T) {
    uint16_t read = Cpu_read(last_absolute_address);
    uint16_t temp = T - read;
    ZF = !temp;
    NF = temp & 0x80;
    CF = (T >= read);
}

void Cpu::CMP() {
    cmp_with(A);
}

void Cpu::CPX() {
    cmp_with(X);
}

void Cpu::CPY() {
    cmp_with(Y);
}

void Cpu::DEC() {
    uint8_t temp = Cpu_read(last_absolute_address);
    temp--;
    ZF = !temp;
    NF = temp & 0x80;
    Cpu_write(last_absolute_address, temp & 0x00FF);
}

void Cpu::DEX() {
    X--;
    ZF = !X;
    NF = X & 0x80;
}

void Cpu::DEY() {
    Y--;
    ZF = !Y;
    NF = Y & 0x80;
}

void Cpu::EOR() {
    uint8_t temp = Cpu_read(last_absolute_address);
    A ^= temp;
    ZF = !A;
    NF = A & 0x80;
}

void Cpu::INC() {
    uint8_t temp = Cpu_read(last_absolute_address);
    temp++;
    ZF = !temp;
    NF = temp & 0x80;
    Cpu_write(last_absolute_address, temp);
}

void Cpu::INX() {
    X++;
    ZF = !X;
    NF = X & 0x80;
}

void Cpu::INY() {
    Y++;
    ZF = !Y;
    NF = Y & 0x80;
}

void Cpu::JMP() {
    PC = last_absolute_address;
}

void Cpu::JSR() {
    PC--;
    push_on_stack((PC >> 8) & 0xFF);
    push_on_stack(PC & 0xFF);
    PC = last_absolute_address;
}

void Cpu::LDA() {
    uint8_t temp = Cpu_read(last_absolute_address);
    A = temp;
    ZF = !A;
    NF = A & 0x80;
}

void Cpu::LDX() {
    uint8_t temp = Cpu_read(last_absolute_address);
    X = temp;
    ZF = !X;
    NF = X & 0x80;
}

void Cpu::LDY() {
    uint8_t temp = Cpu_read(last_absolute_address);
    Y = temp;
    ZF = !Y;
    NF = Y & 0x80;
}

void Cpu::LSR() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = Cpu_read(last_absolute_address);
    CF = temp & 1;
    temp >>= 1;
    ZF = !(temp & 0xFF);
    NF = (temp & 0x80);
    if (!accumulator_mod) {
        Cpu_write(last_absolute_address, temp & 0xFF);
    } else {
        A = temp & 0xFF;
    }
}

void Cpu::NOP() {}

void Cpu::ORA() {
    uint8_t temp = Cpu_read(last_absolute_address);
    A |= temp;
    ZF = !A;
    NF = A & 0x80;
}

void Cpu::PHA() {
    push_on_stack(A);
}

void Cpu::PHP() {
    push_on_stack(flags | 0x10 | 0x20);
    BC = false;
    UNUSED = false;
}

void Cpu::PLA() {
    A = Cpu_read(++SP + 0x0100);
    ZF = !A;
    NF = A & 0x80;
}

void Cpu::PLP() {
    flags = Cpu_read(++SP + 0x0100);
    UNUSED = true;  // for tests
}

void Cpu::ROL() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = Cpu_read(last_absolute_address);
    temp <<= 1;
    temp |= CF;
    CF = (temp >> 8);
    NF = temp & 0x80;
    ZF = !(temp & 0xFF);
    if (!accumulator_mod) {
        Cpu_write(last_absolute_address, temp & 0xFF);
    } else {
        A = temp & 0xFF;
    }
}

void Cpu::ROR() {
    uint16_t temp = A;
    if (!accumulator_mod)
        temp = Cpu_read(last_absolute_address);
    temp += 0x100 * static_cast<uint16_t>(CF);
    CF = temp & 1;
    temp >>= 1;
    NF = temp & 0x80;
    ZF = !(temp & 0xFF);
    if (!accumulator_mod) {
        Cpu_write(last_absolute_address, temp & 0xFF);
    } else {
        A = temp & 0xFF;
    }
}

void Cpu::RTI() {
    SP++;
    flags = Cpu_read(SP + 0x0100);
    BC = false;
    UNUSED = false;  // for tests
    SP++;
    uint16_t temp = Cpu_read(SP + 0x0100);
    PC = temp;
    SP++;
    temp = Cpu_read(SP + 0x0100);
    PC |= temp << 8;
}

void Cpu::RTS() {
    SP++;
    uint16_t temp = Cpu_read(SP + 0x0100);
    PC = temp;
    SP++;
    temp = Cpu_read(SP + 0x0100);
    PC |= temp << 8;
    PC++;
}

void Cpu::SBC() {
    uint16_t temp = Cpu_read(last_absolute_address);
    temp ^= 0xFF;
    uint16_t temp_A = A + temp + CF;
    OF = ((temp_A ^ A) & 0x80 && (temp_A ^ temp) & 0x80);
    A = temp_A & 0xFF;
    CF = temp_A >> 8;
    NF = A & 0x80;
    ZF = !A;
}

void Cpu::SEC() {
    CF = true;
}

void Cpu::SED() {
    DM = true;
}

void Cpu::SEI() {
    ID = true;
}

void Cpu::STA() {
    Cpu_write(last_absolute_address, A);
}

void Cpu::STX() {
    Cpu_write(last_absolute_address, X);
}

void Cpu::STY() {
    Cpu_write(last_absolute_address, Y);
}

void Cpu::TAX() {
    X = A;
    ZF = !X;
    NF = X & 0x80;
}

void Cpu::TAY() {
    Y = A;
    ZF = !Y;
    NF = Y & 0x80;
}

void Cpu::TSX() {
    X = SP;
    ZF = !X;
    NF = X & 0x80;
}

void Cpu::TXA() {
    A = X;
    ZF = !A;
    NF = A & 0x80;
}

void Cpu::TXS() {
    SP = X;
}

void Cpu::TYA() {
    A = Y;
    ZF = !A;
    NF = A & 0x80;
}

void Cpu::throw_exception() {
    uint8_t opcode = Cpu_read(PC - 1);
    throw IncorrectOpcodeError(opcode);
}

void Cpu::tick() {
    if (cycles) {
        cycles--;
        return;
    }
    UNUSED = true;  // for tests
    uint8_t current_opcode = Cpu_read(PC++);
    //    std::cout << std::fixed << std::hex << (int)(PC - 1) << "  " << (int)current_opcode
    //              << "\tA:" << (int)A << " X:" << (int)X << " Y:" << (int)Y << " P:" << (int)flags
    //              << '\n';
    Instruction current_instruction = map_opcodes[current_opcode];
    cycles = map_cycles[current_opcode];
    (this->*current_instruction.addr_mod)();
    (this->*current_instruction.func)();
    accumulator_mod = false;
    UNUSED = true;  // for tests
}

void Cpu::reset() {
    A = 0;
    X = 0;
    Y = 0;
    flags = 0x20;  // for tests set UNUSED flag
    SP = 0xFD;     // The Stack is set at $FD, with two values already stacked ($00, $00).
    last_absolute_address = 0;
    last_relative_address = 0;
    accumulator_mod = false;
    uint16_t temp = Cpu_read(0xFFFC);
    PC = temp;
    temp = Cpu_read(0xFFFD);
    PC |= temp << 8;
    //    PC = 0xC000;  // for tests
    cycles = 8;
}

void Cpu::NMI() {
    push_on_stack(PC >> 8);
    push_on_stack(PC & 0xFF);
    BC = false;
    UNUSED = true;  // for tests
    ID = true;
    push_on_stack(flags);
    uint16_t temp = Cpu_read(0xFFFA);
    PC = temp;
    temp = Cpu_read(0xFFFB);
    PC |= temp << 8;
    cycles = 7;
}

void Cpu::IRQ() {
    if (!ID) {
        push_on_stack(PC >> 8);
        push_on_stack(PC & 0xFF);
        BC = false;
        UNUSED = true;  // for tests
        ID = true;
        push_on_stack(flags);
        uint16_t temp = Cpu_read(0xFFFE);
        PC = temp;
        temp = Cpu_read(0xFFFF);
        PC |= temp << 8;
        cycles = 7;
    }
}

}  // namespace NES
