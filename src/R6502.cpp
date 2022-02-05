#include "R6502.h"
namespace NES{
    CPU::CPU(): map_opcodes{I{&CPU::BRK,&CPU::implicit},
                            I{&CPU::ORA,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ORA,&CPU::zero_page},
                            I{&CPU::ASL,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::PHP,&CPU::implicit},
                            I{&CPU::ORA,&CPU::immediate},
                            I{&CPU::ASL,&CPU::accumulator},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ORA,&CPU::absolute},
                            I{&CPU::ASL,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BPL,&CPU::relative},
                            I{&CPU::ORA,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ORA,&CPU::zero_page_x},
                            I{&CPU::ASL,&CPU::zero_page_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CLC,&CPU::implicit},
                            I{&CPU::ORA,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ORA,&CPU::absolute_x},
                            I{&CPU::ASL,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::JSR,&CPU::absolute},
                            I{&CPU::AND,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BIT,&CPU::zero_page},
                            I{&CPU::AND,&CPU::zero_page},
                            I{&CPU::ROL,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::PLP,&CPU::implicit},
                            I{&CPU::AND,&CPU::immediate},
                            I{&CPU::ROL,&CPU::accumulator},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BIT,&CPU::absolute},
                            I{&CPU::AND,&CPU::absolute},
                            I{&CPU::ROL,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BMI,&CPU::relative},
                            I{&CPU::AND,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::AND,&CPU::zero_page_x},
                            I{&CPU::ROL,&CPU::zero_page_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::SEC,&CPU::implicit},
                            I{&CPU::AND,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::AND,&CPU::absolute_x},
                            I{&CPU::ROL,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::RTI,&CPU::implicit},
                            I{&CPU::EOR,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::EOR,&CPU::zero_page},
                            I{&CPU::LSR,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::PHA,&CPU::implicit},
                            I{&CPU::EOR,&CPU::immediate},
                            I{&CPU::LSR,&CPU::accumulator},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::JMP,&CPU::absolute},
                            I{&CPU::EOR,&CPU::absolute},
                            I{&CPU::LSR,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BVC,&CPU::relative},
                            I{&CPU::EOR,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::EOR,&CPU::zero_page_x},
                            I{&CPU::LSR,&CPU::zero_page_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CLI,&CPU::implicit},
                            I{&CPU::EOR,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::EOR,&CPU::absolute_x},
                            I{&CPU::LSR,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::RTS,&CPU::implicit},
                            I{&CPU::ADC,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ADC,&CPU::zero_page},
                            I{&CPU::ROR,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::PLA,&CPU::implicit},
                            I{&CPU::ADC,&CPU::immediate},
                            I{&CPU::ROR,&CPU::accumulator},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::JMP,&CPU::indirect},
                            I{&CPU::ADC,&CPU::absolute},
                            I{&CPU::ROR,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BVS,&CPU::relative},
                            I{&CPU::ADC,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ADC,&CPU::zero_page_x},
                            I{&CPU::ROR,&CPU::zero_page_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::SEI,&CPU::implicit},
                            I{&CPU::ADC,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::ADC,&CPU::absolute_x},
                            I{&CPU::ROR,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::STA,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::STY,&CPU::zero_page},
                            I{&CPU::STA,&CPU::zero_page},
                            I{&CPU::STX,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::DEY,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::TXA,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::STY,&CPU::absolute},
                            I{&CPU::STA,&CPU::absolute},
                            I{&CPU::STX,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BCC,&CPU::relative},
                            I{&CPU::STA,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::STY,&CPU::zero_page_x},
                            I{&CPU::STA,&CPU::zero_page_x},
                            I{&CPU::STX,&CPU::zero_page_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::TYA,&CPU::implicit},
                            I{&CPU::STA,&CPU::absolute_y},
                            I{&CPU::TXS,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::STA,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::LDY,&CPU::immediate},
                            I{&CPU::LDA,&CPU::indexed_indirect},
                            I{&CPU::LDX,&CPU::immediate},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::LDY,&CPU::zero_page},
                            I{&CPU::LDA,&CPU::zero_page},
                            I{&CPU::LDX,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::TAY,&CPU::implicit},
                            I{&CPU::LDA,&CPU::immediate},
                            I{&CPU::TAX,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::LDY,&CPU::absolute},
                            I{&CPU::LDA,&CPU::absolute},
                            I{&CPU::LDX,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BCS,&CPU::relative},
                            I{&CPU::LDA,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::LDY,&CPU::zero_page_x},
                            I{&CPU::LDA,&CPU::zero_page_x},
                            I{&CPU::LDX,&CPU::zero_page_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CLV,&CPU::implicit},
                            I{&CPU::LDA,&CPU::absolute_y},
                            I{&CPU::TSX,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::LDY,&CPU::absolute_x},
                            I{&CPU::LDA,&CPU::absolute_x},
                            I{&CPU::LDX,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CPY,&CPU::immediate},
                            I{&CPU::CMP,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CPY,&CPU::zero_page},
                            I{&CPU::CMP,&CPU::zero_page},
                            I{&CPU::DEC,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::INY,&CPU::implicit},
                            I{&CPU::CMP,&CPU::immediate},
                            I{&CPU::DEX,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CPY,&CPU::absolute},
                            I{&CPU::CMP,&CPU::absolute},
                            I{&CPU::DEC,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BNE,&CPU::relative},
                            I{&CPU::CMP,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CMP,&CPU::zero_page_x},
                            I{&CPU::DEC,&CPU::zero_page_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CLD,&CPU::implicit},
                            I{&CPU::CMP,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CMP,&CPU::absolute_x},
                            I{&CPU::DEC,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CPX,&CPU::immediate},
                            I{&CPU::SBC,&CPU::indexed_indirect},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CPX,&CPU::zero_page},
                            I{&CPU::SBC,&CPU::zero_page},
                            I{&CPU::INC,&CPU::zero_page},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::INX,&CPU::implicit},
                            I{&CPU::SBC,&CPU::immediate},
                            I{&CPU::NOP,&CPU::implicit},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::CPX,&CPU::absolute},
                            I{&CPU::SBC,&CPU::absolute},
                            I{&CPU::INC,&CPU::absolute},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::BEQ,&CPU::relative},
                            I{&CPU::SBC,&CPU::indirect_indexed},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::SBC,&CPU::zero_page_x},
                            I{&CPU::INC,&CPU::zero_page_x},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::SED,&CPU::implicit},
                            I{&CPU::SBC,&CPU::absolute_y},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::plug,&CPU::throw_exception},
                            I{&CPU::SBC,&CPU::absolute_x},
                            I{&CPU::INC,&CPU::absolute_x},
                            I{&CPU::plug,&CPU::throw_exception}}{}
    IncorrectOpcode::IncorrectOpcode(): std::runtime_error("Incorrect opcode!") {}
    uint8_t CPU::implicit() {
        return 0;
    }

     uint8_t CPU::immediate() {
        return 0;
    }

     uint8_t CPU::zero_page() {
        return 0;
    }

    uint8_t CPU::zero_page_x() {
        return 0;
    }

    uint8_t CPU::zero_page_y() {
        return 0;
    }

    uint8_t CPU::relative() {
        return 0;
    }

    uint8_t CPU::absolute() {
        return 0;
    }

    uint8_t CPU::absolute_x() {
        return 0;
    }

    uint8_t CPU::absolute_y() {
        return 0;
    }

    uint8_t CPU::indirect() {
        return 0;
    }

    uint8_t CPU::indexed_indirect() {
        return 0;
    }

    uint8_t CPU::indirect_indexed() {
        return 0;
    }

    uint8_t CPU::accumulator() {
        return 0;
    }

    void CPU::ADC(uint8_t data) {

    }

    void CPU::AND(uint8_t data) {

    }

    void CPU::ASL(uint8_t data) {

    }

    void CPU::BCC(uint8_t data) {

    }

    void CPU::BCS(uint8_t data) {

    }

    void CPU::BEQ(uint8_t data) {

    }

    void CPU::BIT(uint8_t data) {

    }

    void CPU::BMI(uint8_t data) {

    }

    void CPU::BNE(uint8_t data) {

    }

    void CPU::BPL(uint8_t data) {

    }

    void CPU::BRK() {

    }

    void CPU::BVC(uint8_t data) {

    }

    void CPU::BVS(uint8_t data) {

    }

    void CPU::CLC() {

    }

    void CPU::CLD() {

    }

    void CPU::CLI() {

    }

    void CPU::CLV() {

    }

    void CPU::CMP(uint8_t data) {

    }

    void CPU::CPX(uint8_t data) {

    }

    void CPU::CPY(uint8_t data) {

    }

    void CPU::DEC() {

    }

    void CPU::DEX() {

    }

    void CPU::DEY() {

    }

    void CPU::EOR(uint8_t data) {

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

    void CPU::LDA(uint8_t data) {

    }

    void CPU::LDX(uint8_t data) {

    }

    void CPU::LDY(uint8_t data) {

    }

    void CPU::LSR(uint8_t data) {

    }

    void CPU::NOP() {

    }

    void CPU::ORA(uint8_t data) {

    }

    void CPU::PHA() {

    }

    void CPU::PHP() {

    }

    void CPU::PLA() {

    }

    void CPU::PLP() {

    }

    void CPU::ROL(uint8_t data) {

    }

    void CPU::ROR(uint8_t data) {

    }

    void CPU::RTI() {

    }

    void CPU::RTS() {

    }

    void CPU::SBC(uint8_t data) {

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

    void CPU::plug(){}

    uint8_t CPU::throw_exception() {
        throw IncorrectOpcode();
        return 0;
    }

}

