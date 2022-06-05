#include "apu_envelope.h"

ApuEnvelope::ApuEnvelope(ApuChannelType channel_type, ApuContainer *apu_container)
    : ApuLengthCounter(channel_type, apu_container) {

}

void ApuEnvelope::write_envelope(uint8_t value) {
    use_const_volume = bool((value >> 4) & 1);
    volume = value & 0x0F;
}

void ApuEnvelope::reset_envelope() {
    start = true;
}

uint8_t ApuEnvelope::get_volume() {
    if (length_counter > 0) {
        return (use_const_volume ? volume : decay_counter);
    }
    return 0;
}

void ApuEnvelope::reset(bool soft) {
    ApuLengthCounter::reset(soft);

    use_const_volume = false;
    volume = 0;
    start = false;
    divider = 0;
    decay_counter = 0;
}

void ApuEnvelope::clock_envelope() {
    if (start) {
        start = false;
        divider = volume;
        decay_counter = 15;
    } else {
        if (divider == 0) {
            divider = volume;
            decay_counter = (decay_counter + 15 - 1) & 0x0F;
            // decay - 1 (mod 16)
        } else {
            divider--;
        }
    }
}


