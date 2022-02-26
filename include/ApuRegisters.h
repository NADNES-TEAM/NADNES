#pragma once
#include <cstdint>

namespace NADNES::APU {

struct PulseReg {  // Everything here is accessible only by cpuWrite
    // $4000
    uint8_t duty_loop : 2;
    uint8_t loop_envelope : 1;
    uint8_t constant_volume : 1;
    uint8_t envelope_period : 4;
    // $4001
    uint8_t sweep_unit_enabled : 1;
    uint8_t period : 3;
    uint8_t negative : 1;
    uint8_t shift_count : 3;
    // $4002
    uint8_t timer_low;
    // $4003
    uint8_t length_counter_load : 5;
    uint8_t timer_high : 3;
    // $4004
};

struct TriangleReg {  // Everything here is accessible only by cpuWrite
    // $4008
    uint8_t length_counter_disable : 1;
    uint8_t linear_counter_reload : 7;
    // $4009
    uint8_t unused_4009;
    // $400A
    uint8_t timer_low;
    // $400B
    uint8_t length_counter_load : 5;
    uint8_t timer_high : 3;
    // $400C
};

struct NoiseReg {  // Everything here is accessible only by cpuWrite
    // $400C
    uint8_t unused_bits_400C : 2;
    uint8_t loop_envelope : 1;
    uint8_t constant_volume : 1;
    uint8_t envelope_period : 4;
    // $400E
    uint8_t loop_noise : 1;
    uint8_t unused_bits_400E : 3;
    uint8_t noise_period : 4;
    // $400F
    uint8_t length_counter_load : 5;
    uint8_t unused_bits_400F : 3;
    // $4010
};

struct Dmc {  // Everything here is accessible only by cpuWrite
    // $4010
    uint8_t irq_enable : 1;
    uint8_t loop_sample : 1;
    uint8_t unused_bits_400C : 2;
    uint8_t frequency_index : 4;
    // $4011
    uint8_t unused_bit_4011 : 1;
    uint8_t direct_load : 7;
    // $4012
    uint8_t sample_address;
    // $4013
    uint8_t sample_length;
    // $4014
};

/*
struct {
    // $4015
    uint8_t unused_bits_4015 : 3;
    uint8_t dmc_enable : 1;

    uint8_t noise_length_counter_enable : 1;
    uint8_t triangle_length_counter_enable : 1;
    uint8_t pulse1_length_counter_enable : 1;
    uint8_t pulse2_length_counter_enable : 1;
    // $4016
} StatusWriteReg;
*/

struct StatusReadReg {  // Everything here is accessible by cpuWrite AND cpuRead
    // $4015
    uint8_t dmc_interrupt : 1;
    uint8_t frame_interrupt : 1;
    uint8_t unused_bit_4016 : 1;
    uint8_t dmc_active : 1;

    uint8_t noise_length_counter_enable : 1;
    uint8_t triangle_length_counter_enable : 1;
    uint8_t pulse1_length_counter_enable : 1;
    uint8_t pulse2_length_counter_enable : 1;
    // $4016
};

struct FrameCounterReg {
    // $4017
    uint8_t mode : 1;
    uint8_t irq_inhibit : 1;
    // $4018
};
}  // namespace NADNES::APU
