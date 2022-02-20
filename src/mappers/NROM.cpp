#include "mappers/NROM.h"
#include "mapper.h"

namespace NES {
NROMMapper::NROMMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr)
    : mirror_type(mirror_type_), PRG_banks_count(prg), CHR_banks_count(chr) {
    if (prg > 2 || chr != 1) {
        throw InvalidMapperConfigurationError();
    }
}

uint16_t NROMMapper::map_read_from_CPU(uint16_t address) const {
    if (address >= 0x8000) {
        return (address % (PRG_banks_count == 1 ? 0x4000 : 0x8000));
    } else {
        throw AddressOutOfBoundsError(address, "CPU");
    }
}

uint16_t NROMMapper::map_PPU_address(uint16_t address) const {
    if (address >= 0x3F00) {
        throw AddressOutOfBoundsError(address, "PPU");
    }
    if (address < 0x2000) {
        return address;
    } else {
        if (mirror_type == Mirroring::Horizontal) {
            return (address % 0x0400 + 0x0800 * (address >= 0x2800) + 0x2000);
        } else {
            return (address % 0x0800 + 0x2000);
        }
    }
}
}  // namespace NES
