#ifndef NADNES_CARTRIDGE_H
#define NADNES_CARTRIDGE_H

#include <cstdint>
#include <string>
#include "mapper.h"
#include <stdexcept>
#include <memory>
#include <vector>

namespace NES {
    struct PrgRomInterface {
        virtual uint8_t PRG_ROM_read(uint16_t address) = 0;
        virtual ~PrgRomInterface() = default;
    };

    struct ChrRomInterface {
        virtual uint8_t CHR_ROM_read(uint16_t address) = 0;
        virtual ~ChrRomInterface() = default;
    };


    class Cartridge : public PrgRomInterface, public ChrRomInterface {
        static const uint16_t PRG_ROM_BANK_SIZE = 1 << 14;
        static const uint16_t CHR_ROM_BANK_SIZE = 1 << 13;
        std::unique_ptr<AbstractMapper> mapper;
        std::vector<uint8_t> PRG_ROM;
        std::vector<uint8_t> CHR_ROM;
    public:
        uint8_t PRG_ROM_read(uint16_t address) override;
        uint8_t CHR_ROM_read(uint16_t address) override;
        explicit Cartridge(const std::string &filename);
    };


}  // namespace NES

#endif  // NADNES_CARTRIDGE_H
