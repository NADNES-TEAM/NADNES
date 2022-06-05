#pragma once

#include "apu_fwd.h"
#include "nes_devices/R6502.h"

class ApuContainer {
public:
    ApuContainer(Apu *apu, ApuMixer *mixer, NES::Cpu *cpu) : apu(apu), mixer(mixer), cpu(cpu) {
    }
    Apu *get_apu() {
        return apu;
    }
    ApuMixer *get_mixer() {
        return mixer;
    }
    NES::Cpu *get_cpu() {
        return cpu;
    }

private:
    Apu *apu{};
    ApuMixer *mixer{};
    NES::Cpu *cpu{};
};