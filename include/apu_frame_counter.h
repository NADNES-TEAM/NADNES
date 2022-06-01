#pragma once
#include <array>
#include <cstdint>

enum class Frame {
    NONE,
    QUARTER,
    HALF,
};

enum class StepMode {
    FOUR_STEP = 0,
    FIVE_STEP = 1,
};

struct ApufcRunResult {
    int actually_ran{};
    bool need_to_clock_half{};
    bool need_to_clock_quarter{};
};

struct ApuFrameCounter {
    ApuFrameCounter();
    void reset(bool reset_mode);
    void write_data(uint16_t addr, uint8_t data, uint64_t cpu_cycle_count);
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

    int skip_clock;
};
