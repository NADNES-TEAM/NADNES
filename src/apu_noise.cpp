#include "apu_noise.h"
#include "apu.h"

const std::array<uint16_t, 16> ApuNoise::noise_table{
    {4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068}};

ApuNoise::ApuNoise(ApuChannelType channel_type, ApuContainer *apu_container)
    : ApuEnvelope(channel_type, apu_container) {}

bool ApuNoise::is_muted() const {
    return bool(shift_reg & 1);
}

void ApuNoise::clock() {
    uint16_t new_bit = (shift_reg & 1) ^ ((shift_reg >> (noise_mode ? 6 : 1)) & 1);
    shift_reg = (shift_reg >> 1) + (new_bit << 14);

    if (is_muted()) {
        add_output(0);
    } else {
        add_output(get_volume());
    }
}

void ApuNoise::reset(bool soft) {
    ApuEnvelope::reset(soft);

    period = noise_table[0] - 1;
    shift_reg = 1;
    noise_mode = false;
}

void ApuNoise::CPU_write(uint16_t addr, uint8_t value) {
    apu_container->get_apu()->run();

    switch (addr & 3) {
        case 0: {
            init_length_counter((value >> 5) & 1);
            write_envelope(value);
        } break;
        case 2: {
            period = noise_table[value & 0xF] - 1;
            noise_mode = ((value >> 7) & 1);
        } break;
        case 3: {
            load_length_counter(value >> 3);
            reset_envelope();
        } break;
    }
}
