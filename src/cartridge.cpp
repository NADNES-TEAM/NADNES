#include "cartridge.h"
#include <fstream>
#include <iostream>
#include "NROM.h"


namespace NES {
    union HeaderFlags {
        HeaderFlags(uint16_t all_) : all(all_) {};
        struct {
            uint16_t mirror_type: 1;
            uint16_t PRG_RAM: 1;
            uint16_t trainer: 1;
            uint16_t four_screen: 1;
            uint16_t mapper_lsb: 4;
            uint16_t console_type: 2;
            uint16_t nes_2_0: 2;
            uint16_t mapper_msb: 4;
        };
        uint16_t all;
    };

    enum class MapperTypes : uint8_t {
        NROM = 0
    };

    std::unique_ptr<AbstractMapper>
    construct_mapper(uint8_t PRG_banks_count, uint8_t CHR_banks_count, HeaderFlags flags) {
        uint8_t mapper_number = (flags.mapper_msb << 4) + flags.mapper_lsb;
        MapperTypes mapper_type{mapper_number};
        switch (mapper_type) {
            case MapperTypes::NROM:
                return std::make_unique<NROMMapper>(NROMMapper(Mirroring{flags.mirror_type},
                                                               PRG_banks_count,
                                                               CHR_banks_count));
            default:
                throw UnknownMapperType(mapper_number);
        }


    }

    Cartridge::Cartridge(const std::string &filename) {
        std::ifstream rom_file(filename, std::ios::binary);
        if (!rom_file.is_open()) {
            throw UnableToOpenFile(filename);
        }
        rom_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
        char header[5];
        rom_file.read(header, 4);
        if (std::string(header) != "NES\x1a") {
            throw InvalidHeaderFormat();
        }
        uint8_t PRG_banks_count = rom_file.get();
        uint8_t CHR_banks_count = rom_file.get();
        uint16_t header_flags = rom_file.get() + (rom_file.get() << 8);
        mapper = construct_mapper(PRG_banks_count, CHR_banks_count, header_flags);
        rom_file.seekg(8, std::ios_base::cur);

        uint16_t PRG_ROM_size = PRG_ROM_BANK_SIZE * PRG_banks_count;
        PRG_ROM.resize(PRG_ROM_size);
        for (uint16_t i = 0; i < PRG_ROM_size; i++) {
            PRG_ROM[i] = rom_file.get();
        }

        uint16_t CHR_ROM_size = CHR_ROM_BANK_SIZE * CHR_banks_count;
        CHR_ROM.resize(CHR_ROM_size);
        for (uint16_t i = 0; i < CHR_ROM_size; i++) {
            CHR_ROM[i] = rom_file.get();
        }
    }

    uint8_t Cartridge::PRG_ROM_read(uint16_t address) {
        return PRG_ROM[mapper->map_PRG_ROM_address(address)];
    }

    uint8_t Cartridge::CHR_ROM_read(uint16_t address) {
        return CHR_ROM[mapper->map_CHR_ROM_address(address)];
    }

}  // namespace NES