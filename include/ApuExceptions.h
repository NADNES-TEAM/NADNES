#pragma once
#include <stdexcept>
#include <string>
#include <sstream>

namespace NADNESS::APU {

std::string get_hex(std::uint16_t addr);

struct BadApuRead : std::runtime_error {
    explicit BadApuRead(uint16_t);
};

struct BadApuWrite : std::runtime_error {
    explicit BadApuWrite(uint16_t addr);
};

}
