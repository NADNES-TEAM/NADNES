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

struct ApuFrameCounter {
    ApuFrameCounter();
    void reset(bool reset_mode);
    void write_data(uint16_t addr, uint8_t data, uint64_t cpu_cycle_count);

private:
    const std::array<std::array<int, 6>, 2> m_cycles{
        {{7457, 14913, 22371, 29828, 29829, 29830},
         {7457, 14913, 22371, 29829, 37281, 37282}}};

    bool inhibit_irq;
    StepMode step_mode;
    int step;
    int prev_cycle;

    int skip_write_cycles;
    uint8_t write_value;

    int skip_clock;
};
