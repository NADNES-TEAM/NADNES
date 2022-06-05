#include "apu_frame_counter.h"
#include <cassert>
#include <limits>
#include "apu.h"

ApuFrameCounter::ApuFrameCounter(ApuContainer *apu_container) : apu_container(apu_container) {
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

void ApuFrameCounter::write_data(uint16_t addr, uint8_t data) {
    assert(addr == 0x4017);
    write_value = data;
    inhibit_irq = bool((write_value >> 6) & 1);
    if (apu_container->get_apu()->get_clock_count() & 1) {  // wait next odd cycle
        skip_write_cycles = 4;
    } else {
        skip_write_cycles = 3;
    }
    if (inhibit_irq) {
        // TODO clear interrupts in CPU
    }
}

ApufcRunResult ApuFrameCounter::run_cycles(int cycles_available) {
    ApufcRunResult res{};

    if (prev_cycle + cycles_available >= m_cycles[int(step_mode)][step]) {
        // TODO: set IRQ if needed

        auto frame = m_frames[(int)step_mode][step];
        if (frame != Frame::NONE && skip_clock == 0) {
            if (frame == Frame::HALF) {
                res.need_to_clock_half = true;
            } else {
                res.need_to_clock_quarter = true;
            }
            skip_clock = 2;  // wait till next odd cpu cycle
        }
        res.actually_ran = m_cycles[(int)step_mode][step] - prev_cycle;
        // [prev_cycle; m_cycles[..][..])
        step++;
        // If it was last step, then loop from the start
        if (step == 6) {
            step = 0;
            prev_cycle = 0;
        } else {
            prev_cycle += res.actually_ran;
        }
    } else {  // Nothing changed, so
        res.actually_ran = cycles_available;
        prev_cycle += cycles_available;
    }

    // now we apply everything that came from write_data
    if (write_value >= 0) {  // if IRQ = 0 and mode = 0 => no write
        if (--skip_write_cycles == 0) {
            // everything skipped
            step_mode = StepMode((write_value >> 7) & 1);

            // The following code sets this place unreachable and resets some variables
            step = 0;
            prev_cycle = 0;
            skip_write_cycles = std::numeric_limits<int>::max();  // --skip_write_cycles is negative
            write_value = 0xFF;                                   // "-1" in uint8_t

            if (step_mode == StepMode::FIVE_STEP && skip_clock == 0) {
                res.need_to_clock_half = true;
                skip_write_cycles = 2;
            }
        }
    }

    if (skip_write_cycles > 0) {
        --skip_write_cycles;
    }

    return res;
}
bool ApuFrameCounter::need_to_run(uint32_t cycles_to_run) {
    return write_value >= 0 || skip_write_cycles > 0 ||
           (prev_cycle + (int32_t)cycles_to_run >= m_cycles[(int)step_mode][step] - 1);
}
