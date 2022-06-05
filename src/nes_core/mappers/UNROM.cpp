#include "mappers/UNROM.h"

namespace NES {

UnromMapper::UnromMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr)
    : mirror_type(mirror_type_), high_bank_select(prg - 1) {}

void UnromMapper::map_write_from_CPU(uint16_t address, uint8_t data) {
    if (address < 0x8000) {
        throw AddressOutOfBoundsError(address, "CPU");
    }
    low_bank_select = data & 0x0F;
}

void UnromMapper::reset(ResetToken) {
    low_bank_select = 0;
}

uint32_t UnromMapper::map_read_from_CPU(uint16_t address) const {
    if (address < 0x8000) {
        throw AddressOutOfBoundsError(address, "CPU");
    } else if (address < 0xC000) {
        return (address - 0x8000) + 0x4000 * low_bank_select;
    } else {
        return (address - 0xC000) + 0x4000 * high_bank_select;
    }
}

uint16_t UnromMapper::map_PPU_address(uint16_t address) const {
    if (address >= 0x3F00) {
        throw AddressOutOfBoundsError(address, "PPU");
    }
    if (address < 0x2000) {
        return address;
    } else {
        address -= 0x2000;
        if (mirror_type == Mirroring::Horizontal) {
            return (address % 0x0400 + 0x0800 * (address >= 0x2800) + 0x2000);
        } else {
            return (address % 0x0800 + 0x2000);
        }
    }
}

void UnromMapper::save(std::ostream &file) {
    file.write(reinterpret_cast<char *>(&low_bank_select), sizeof low_bank_select);
}

void UnromMapper::load(std::istream &file) {
    file.read(reinterpret_cast<char *>(&low_bank_select), sizeof low_bank_select);
}

}  // namespace NES