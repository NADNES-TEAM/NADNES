#include "apu_pulse.h"
#include "apu.h"

const std::array<uint8_t, 4> ApuPulse::duty_seq = {{
    0b1000'0000,
    0b1100'0000,
    0b1111'0000,
    0b0011'1111
}};

ApuPulse::ApuPulse(ApuChannelType channel_type, ApuContainer *apu_container)
    : ApuEnvelope(channel_type, apu_container) {

}

void ApuPulse::sweep_write(uint8_t value) {
    sweep_enabled = (value & 0x80) == 0x80;
    sweep_negate = (value & 0x08) == 0x08;

    //The divider's period is set to P + 1
    sweep_period = ((value & 0x70) >> 4) + 1;
    sweep_shift = (value & 0x07);

    update_target_period();

    sweep_reload = true;
}

void ApuPulse::update_target_period() {
    uint16_t shiftResult = (real_period >> sweep_shift);
    if(sweep_negate) {
        sweep_target_period = real_period - shiftResult;
        if(channel_type == ApuChannelType::PULSE1) {
            sweep_target_period--;
        }
    } else {
        sweep_target_period = real_period + shiftResult;
    }
}

void ApuPulse::set_period(uint16_t new_period) {
    real_period = new_period;
    period = (real_period * 2) + 1;
    update_target_period();
}

void ApuPulse::update_output() {
    if (is_muted()) {
        add_output(0);
    } else {
        add_output((duty_seq[duty] >> duty_pos) * get_volume());
    }
}

bool ApuPulse::is_muted() const {
    return real_period < 8 || (!sweep_negate && sweep_target_period > 0x7FF);
}

void ApuPulse::clock() {
    duty_pos = (duty_pos - 1) & 0x7;
    update_output();
}

void ApuPulse::reset(bool soft) {
    ApuEnvelope::reset(soft);

    duty = 0;
    duty_pos = 0;

    real_period = 0;

    sweep_enabled = false;
    sweep_period = 0;
    sweep_negate = false;
    sweep_shift = 0;
    sweep_reload = false;
    sweep_divider = 0;
    sweep_target_period = 0;
    update_target_period();
}

void ApuPulse::CPU_write(uint16_t addr, uint8_t value) {
    apu_container->get_apu()->run();

    switch (addr & 3) {
        case 0: {  // 4000 + 4004
            write_halt(bool(value & 0x20));
            write_envelope(value);

            duty = (value >> 6);
        }break;
        case 1: {  // 4001 + 4005
            sweep_write(value);
        } break;

        case 2: {  // 4002 + 4006
            set_period((real_period & 0x0700) | value);
            // TODO why?
        } break;

        case 3: {  // 4003 + 4007
            set_length_counter(value >> 3);

            set_period((real_period & 0xFF) | ((value & 0x07) << 8));

            // sequencer is restarted at the first value of the current sequence
            duty_pos = 0;

            // envelope is also restarted
            reset_envelope();
        } break;
    }
    update_output();
}

void ApuPulse::clock_sweep() {
    sweep_divider--;
    if (sweep_divider == 0) {
        if (sweep_shift > 0 && sweep_enabled && real_period >= 8 && sweep_target_period <= 0x7FF) {
            set_period(sweep_target_period);
        }
        sweep_divider = sweep_period;

        if (sweep_reload) {
            sweep_divider = sweep_period;
            sweep_reload = false;
        }
    }
}
