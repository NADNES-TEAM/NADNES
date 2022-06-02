#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include "ApuRegisters.h"

class Apu {
public:
    void cpuWrite(std::uint16_t addr, std::uint8_t value);

    uint8_t cpuRead(uint16_t addr);

    int get_clock_count();
};
