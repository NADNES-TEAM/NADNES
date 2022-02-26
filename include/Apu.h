#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include "ApuRegisters.h"

namespace NADNES::APU {

#pragma pack(push, 1)
class Apu {
public:
    void cpuWrite(std::uint16_t addr, std::uint8_t value);

    uint8_t cpuRead(uint16_t addr);

private:
    // $4000
    PulseReg pulseReg1;
    // $4004
    PulseReg pulseReg2;
    // $4008
    TriangleReg triangleReg;
    // $400C
    NoiseReg noiseReg;
    // $400F
    Dmc dmc;
    // $4014
    uint8_t unused_byte_4014;
    // $4015
    StatusReadReg statusReadReg;
    // $4016
    uint8_t unused_byte4016;
    // $4017
    FrameCounterReg frameCounterReg;
    // $4018
};
#pragma pack(pop)

}  // namespace NADNES::APU
