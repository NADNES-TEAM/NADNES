#include "mappers/NROM.h"
#include "mapper.h"

namespace NES {
    NROMMapper::NROMMapper(Mirroring mirror_type_, uint8_t prg, uint8_t chr) : mirror_type(mirror_type_),
                                                                               PRG_banks_count(prg),
                                                                               CHR_banks_count(chr) {
        if (prg > 2 || chr != 1) {
            throw InvalidMapperConfiguration();
        }
    }
    uint16_t NROMMapper::map_PRG_ROM_address(uint16_t address) {
        if (address >= 0x8000){
            return (address % 0x8000) % 0x4000;
        } else {
            throw AddressOutOfBounds(address, "PRG");
        }
    }

    uint16_t NROMMapper::map_CHR_ROM_address(uint16_t address) {
        if (address <= 0x2000) {
            return address;
        } else {
            throw AddressOutOfBounds(address, "CHR");
        }
    }
}