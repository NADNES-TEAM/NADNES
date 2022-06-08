#include "apu_triangle.h"
#include "apu.h"

ApuTriangle::ApuTriangle(ApuChannelType channel_type, ApuContainer *apu_container)
    : ApuLengthCounter(channel_type, apu_container) {}

const std::array<uint8_t, 32> ApuTriangle::seq = {{15, 14, 13, 12, 11, 10, 9,  8,  7,  6, 5,
                                                   4,  3,  2,  1,  0,  0,  1,  2,  3,  4, 5,
                                                   6,  7,  8,  9,  10, 11, 12, 13, 14, 15}};

void ApuTriangle::clock() {
    if (length_counter > 0 && linear_counter > 0) {
        seq_pos = (seq_pos + 1) & 31;
        add_output(seq[seq_pos]);
    }
}

void ApuTriangle::reset(bool soft) {
    ApuLengthCounter::reset(soft);

    linear_counter = 0;
    linear_counter_reload = 0;
    linear_reload_flag = false;
    linear_control_flag = false;

    seq_pos = 0;
}

void ApuTriangle::CPU_write(uint16_t addr, uint8_t value) {
    apu_container->get_apu()->run();

    switch (addr & 3) {
        case 0: {
            linear_control_flag = bool((value >> 7) & 1);
            linear_counter_reload = value & 0x7F;

            init_length_counter(linear_control_flag);
        } break;
        case 1: {
            period &= ~0x00FF;
            period |= value;
        } break;
        case 3: {
            load_length_counter(value >> 3);
            period &= ~0xFF00;
            period |= (value & 0x07) << 8;

            linear_reload_flag = true;
        } break;
    }
}

void ApuTriangle::clock_linear_counter() {
    if (linear_reload_flag) {
        linear_counter = linear_counter_reload;
    } else if (linear_counter > 0) {
        linear_counter--;
    }

    if (!linear_control_flag) {
        linear_reload_flag = false;
    }
}
