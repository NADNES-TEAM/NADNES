#pragma once
#include <array>
#include <cstdint>

#include "apu_enums.h"
#include "apu_container.h"

struct ApufcRunResult {
    int actually_ran{};
    bool need_to_clock_half{};
    bool need_to_clock_quarter{};
};

struct ApuFrameCounter {
    explicit ApuFrameCounter(ApuContainer *apu_container);
    void reset(bool reset_mode);
    void write_data(uint16_t addr, uint8_t data);
    ApufcRunResult run_cycles(int cycles_available);  // cycles_available - how many cpu
    // need to run

private:
    const std::array<std::array<int, 6>, 2> m_cycles{
        {{7457, 14913, 22371, 29828, 29829, 29830}, {7457, 14913, 22371, 29829, 37281, 37282}}};
    std::array<std::array<Frame, 6>, 2> m_frames{
        {{Frame ::QUARTER, Frame ::HALF, Frame ::QUARTER, Frame ::NONE, Frame ::HALF, Frame ::NONE},
         {Frame ::QUARTER,
          Frame ::HALF,
          Frame ::QUARTER,
          Frame ::NONE,
          Frame ::HALF,
          Frame ::NONE}}};

    bool inhibit_irq;  // TODO
    StepMode step_mode;
    int step;        // index in m_cycles
    int prev_cycle;  // real value (not index)

    int skip_write_cycles;  // how much to wait
    uint8_t write_value;  // first we write, then wait
    ApuContainer *apu_container;

    int skip_clock;
};
