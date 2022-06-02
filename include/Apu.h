#pragma once
#include <array>
#include <cassert>
#include <cstdint>

class Apu {
public:
    void cpuWrite(std::uint16_t addr, std::uint8_t value);

    uint8_t cpuRead(uint16_t addr);

    uint32_t get_clock_count();


};
