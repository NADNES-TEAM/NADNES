#include <cassert>
#include "apu_frame_counter.h"

ApuFrameCounter::ApuFrameCounter() {
    reset(true);
}

void ApuFrameCounter::reset(bool reset_mode) {
    if (reset_mode) {
        step_mode = StepMode::FOUR_STEP;
    }

    skip_write_cycles = 3;  // We should emulate like write_data was called
    // but with a small delay
    // write_value is basically mode
    write_value = (step_mode == StepMode::FOUR_STEP ? 0 : (1 << 7));
    inhibit_irq = false;
    skip_clock = 0;
    step = 0;
    prev_cycle = 0;
}

void ApuFrameCounter::write_data(uint16_t addr, uint8_t data, uint64_t cpu_cycle_count) {
    assert(addr == 0x4017);
    write_value = data;
    inhibit_irq = bool((write_value >> 6) & 1);
    if (cpu_cycle_count & 1) {  // wait next odd cycle
        skip_write_cycles = 4;
    } else {
        skip_write_cycles = 3;
    }
    if (inhibit_irq) {

    }
}
