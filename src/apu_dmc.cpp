#include "apu_dmc.h"

const std::array<uint16_t, 16> ApuDmc::period_table{
    {428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106, 84, 72, 54}};

ApuDmc::ApuDmc(ApuChannelType type, ApuContainer *apu_container)
    : ApuBaseChannel(type, apu_container) {}

void ApuDmc::init_sample() {
    cur_addr = sample_addr;
    bytes_remaining = sample_len;
    need_run = bool(bytes_remaining);
}

void ApuDmc::clock() {
    if (!silence_flag) {
        if (shift_reg & 1) {
            if (output_lvl <= 125) {
                output_lvl += 2;
            }
        } else if (output_lvl >= 2) {
            output_lvl -= 2;
        }
        shift_reg >>= 1;
    }

    --bits_remaining;
    if (0 == bits_remaining) {
        bits_remaining = 8;
        if (buffer_empty) {
            silence_flag = true;
        } else {
            silence_flag = false;
            shift_reg = read_buffer;
            buffer_empty = true;
            // TODO start Dmc transfer
        }
    }

    add_output(output_lvl);
}

void ApuDmc::reset(bool soft) {
    ApuBaseChannel::reset(soft);

    if (!soft) {
        sample_addr = 0xC000;
        sample_len = 1;
    }

    output_lvl = 0;
    irq_enabled = false;
    loop_flag = false;

    cur_addr = 0;
    bytes_remaining = 0;
    read_buffer = 0;
    buffer_empty = true;

    shift_reg = 0;
    bits_remaining = 8;
    silence_flag = true;
    need_run = false;

    last_value_4011 = 0;

    period = period_table[0] - 1;

    timer = period;
}

void ApuDmc::set_enabled(bool enabled) {
    if (!enabled) {
        bytes_remaining = 0;
        need_run = false;
    } else if (bytes_remaining == 0) {
        init_sample();
    }
}
bool ApuDmc::need_to_run() {
    return false;
    // TODO
}
bool ApuDmc::irq_pending(uint32_t cycles_to_run) {
    return false;
    // TODO
}
