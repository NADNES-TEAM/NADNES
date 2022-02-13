#include "R6502.h"

#include <utility>
namespace NES {


    CPU::CPU() : map_opcodes{I{&CPU::BRK, &CPU::implicit},
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
                 map_cycles{7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0, 2, 2, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
                        6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
                        6, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 3, 4, 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
                        6, 6, 0, 0, 0, 3, 5, 0, 4, 2, 2, 0, 5, 4, 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
                        0, 6, 0, 0, 3, 3, 3, 0, 2, 0, 2, 0, 4, 4, 4, 0, 2, 6, 0, 0, 4, 4, 4, 0, 2, 5, 2, 0, 0, 5, 0, 0,
                        2, 6, 2, 0, 3, 3, 3, 0, 2, 2, 2, 0, 4, 4, 4, 0, 2, 5, 0, 0, 4, 4, 4, 0, 2, 4, 2, 0, 4, 4, 4, 0,
                        2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0, 0, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
                        2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0, 2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7,
                        0} {}

    IncorrectOpcode::IncorrectOpcode() : std::runtime_error("Incorrect opcode!") {}

    void CPU::implicit() {}//just do nothing

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
        if((last_absolute_address>>8) !=((last_absolute_address-X)>>8))
            cycles++;
    }

    void CPU::absolute_y() {
        absolute();
        last_absolute_address += Y;
        if((last_absolute_address>>8) !=((last_absolute_address-Y)>>8))
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
            last_absolute_address = ((*bus).mem_read(temp_address & 0xFF00));
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
        uint8_t LSB = (*bus).mem_read((temp_address) & 0x00FF);
        last_absolute_address = (*bus).mem_read((temp_address + 1) & 0x00FF);
        last_absolute_address <<= 8;
        last_absolute_address += LSB;
        last_absolute_address += Y;
        if((last_absolute_address>>8) !=((last_absolute_address-Y)>>8))
            cycles++;
    }

    void CPU::accumulator() {
        accumulator_mod = true;
    }

    void CPU::ADC() {

    }

    void CPU::AND() {

    }

    void CPU::ASL() {

    }

    void CPU::BCC() {

    }

    void CPU::BCS() {

    }

    void CPU::BEQ() {

    }

    void CPU::BIT() {

    }

    void CPU::BMI() {

    }

    void CPU::BNE() {

    }

    void CPU::BPL() {

    }

    void CPU::BRK() {

    }

    void CPU::BVC() {

    }

    void CPU::BVS() {

    }

    void CPU::CLC() {

    }

    void CPU::CLD() {

    }

    void CPU::CLI() {

    }

    void CPU::CLV() {

    }

    void CPU::CMP() {

    }

    void CPU::CPX() {

    }

    void CPU::CPY() {

    }

    void CPU::DEC() {

    }

    void CPU::DEX() {

    }

    void CPU::DEY() {

    }

    void CPU::EOR() {

    }

    void CPU::INC() {

    }

    void CPU::INX() {

    }

    void CPU::INY() {

    }

    void CPU::JMP() {

    }

    void CPU::JSR() {

    }

    void CPU::LDA() {

    }

    void CPU::LDX() {

    }

    void CPU::LDY() {

    }

    void CPU::LSR() {

    }

    void CPU::NOP() {

    }

    void CPU::ORA() {

    }

    void CPU::PHA() {

    }

    void CPU::PHP() {

    }

    void CPU::PLA() {

    }

    void CPU::PLP() {

    }

    void CPU::ROL() {

    }

    void CPU::ROR() {

    }

    void CPU::RTI() {

    }

    void CPU::RTS() {

    }

    void CPU::SBC() {

    }

    void CPU::SEC() {

    }

    void CPU::SED() {

    }

    void CPU::SEI() {

    }

    void CPU::STA() {

    }

    void CPU::STX() {

    }

    void CPU::STY() {

    }

    void CPU::TAX() {

    }

    void CPU::TAY() {

    }

    void CPU::TSX() {

    }

    void CPU::TXA() {

    }

    void CPU::TXS() {

    }

    void CPU::TYA() {

    }

    void CPU::throw_exception() {
        throw IncorrectOpcode();
    }

    void CPU::connect_bus(std::shared_ptr<Bus> bus_) {
        bus = std::move(bus_);
    }

    void CPU::clock() {

    }

    void CPU::reset() {

    }

}

