#include "nes_devices/cartridge.h"
#include <fstream>
#include <iostream>
#include "NROM.h"
#include "UNROM.h"

namespace NES {
enum class MapperTypes : uint8_t { NROM = 0, UNROM = 2 };

[[nodiscard]] std::unique_ptr<AbstractMapper> construct_mapper(const iNesHeader &header) {
    uint8_t mapper_number;
    if (header.nes_2_0_id == 2) {
        mapper_number = (header.mapper_high_bits << 4) + header.mapper_low_bits;
    } else {
        mapper_number = header.mapper_low_bits;
    }
    MapperTypes mapper_type{mapper_number};
    switch (mapper_type) {
        case MapperTypes::NROM:
            return std::make_unique<NromMapper>(Mirroring(header.mirror_type),
                                                header.PRG_ROM_banks_lsb,
                                                header.CHR_ROM_banks_lsb);
        case MapperTypes::UNROM:
            return std::make_unique<UnromMapper>(Mirroring(header.mirror_type),
                                                 header.PRG_ROM_banks_lsb,
                                                 header.CHR_ROM_banks_lsb);
        default: throw UnknownMapperTypeError(mapper_number);
    }
}

Cartridge::Cartridge(const std::string &filename) : CHR_RAM(CHR_RAM_size) {
    std::ifstream rom_file(filename, std::ios::binary);
    if (!rom_file.is_open()) {
        throw UnableToOpenFileError(filename);
    }
    rom_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    iNesHeader header;
    rom_file.read(reinterpret_cast<char *>(&header), 16);
    // first 4 bytes should be "NES<EOF>" in little-endian, <EOF> = 0x1A
    if (header.id_str != ((0x1A << 24) + ('S' << 16) + ('E' << 8) + 'N')) {
        throw InvalidHeaderFormatError();
    }

    mapper = construct_mapper(header);

    uint8_t PRG_banks_count = header.PRG_ROM_banks_lsb;
    uint32_t PRG_ROM_size = PRG_ROM_BANK_SIZE * PRG_banks_count;
    PRG_ROM.resize(PRG_ROM_size);
    rom_file.read(reinterpret_cast<char *>(&PRG_ROM.front()), PRG_ROM_size);

    uint8_t CHR_banks_count = (header.CHR_ROM_size_msb << 4) + header.CHR_ROM_banks_lsb;
    if (CHR_banks_count == 0) {
        CHR_ROM.resize(1 << 13);
        use_CHR_ROM_as_RAM = true;
    } else {
        CHR_ROM.resize(CHR_ROM_BANK_SIZE * CHR_banks_count);
    }
    rom_file.read(reinterpret_cast<char *>(&CHR_ROM.front()), CHR_ROM_BANK_SIZE * CHR_banks_count);

    m_hash = 0;
    for (auto i : PRG_ROM) {
        m_hash = (m_hash ^ i) * 239;
    }
    for (auto i : CHR_ROM) {
        m_hash = (m_hash ^ i) * 239;
    }
}

uint8_t Cartridge::CPU_read(uint16_t address) const {
    return PRG_ROM[mapper->map_read_from_CPU(address)];
}

uint8_t Cartridge::PPU_read(uint16_t address) const {
    uint16_t mapped_address = mapper->map_PPU_address(address);
    if (mapped_address >= CHR_ROM.size()) {
        return CHR_RAM[mapped_address % CHR_ROM.size()];
    } else {
        return CHR_ROM[mapped_address];
    }
}

void Cartridge::CPU_write(uint16_t address, uint8_t data) {
    mapper->map_write_from_CPU(address, data);
}

void Cartridge::PPU_write(uint16_t address, uint8_t data) {
    uint16_t mapped_address = mapper->map_PPU_address(address);
    if (mapped_address >= CHR_ROM.size()) {
        CHR_RAM[mapped_address % CHR_ROM.size()] = data;
    } else if (use_CHR_ROM_as_RAM) {
        CHR_ROM[mapped_address] = data;
    } else {
        throw WritingToRomError();
    }
}
uint8_t *Cartridge::get_ROM() {
    return &PRG_ROM[0];
}
size_t Cartridge::get_ROM_size() {
    return PRG_ROM.size();
}
uint64_t Cartridge::get_hash() const {
    return m_hash;
}

void Cartridge::save(std::ostream &file) {
    file.write(reinterpret_cast<char *>(&CHR_RAM[0]), CHR_RAM_size);
    if (use_CHR_ROM_as_RAM) {
        file.write(reinterpret_cast<char *>(&CHR_ROM[0]), CHR_ROM.size());
    }
    mapper->save(file);
}

void Cartridge::load(std::istream &file) {
    file.read(reinterpret_cast<char *>(&CHR_RAM[0]), CHR_RAM_size);
    if (use_CHR_ROM_as_RAM) {
        file.read(reinterpret_cast<char *>(&CHR_ROM[0]), CHR_ROM.size());
    }
    mapper->load(file);
}

void Cartridge::reset(const ResetToken &token) {
    mapper->reset(token);
}

}  // namespace NES
