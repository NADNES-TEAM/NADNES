#ifndef NADNES_CARTRIDGE_H
#define NADNES_CARTRIDGE_H

#include <cstdint>
#include <string>

namespace NES {

struct CartridgeInterface {
    virtual void load(const std::string &file_name) = 0;

    // mapper uses these methods
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
};

class Cartridge : CartridgeInterface {
};

}  // namespace NES

#endif  // NADNES_CARTRIDGE_H
