#include "apu_length_counter.h"
#include "apu_enums.h"

const std::array<uint8_t, 32> ApuChannelWithLengthCounter::length_table{
    {10, 254, 20, 2,  40, 4,  80, 6,  160, 8,  60, 10, 14, 12, 26, 14,
     12, 16,  24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30}};

ApuChannelWithLengthCounter::ApuChannelWithLengthCounter(ApuChannelType channel_type,
                                                         ApuContainer *apu_container)
    : ApuBaseChannel(channel_type, apu_container) {
    ApuChannelWithLengthCounter::reset(false);
}

void ApuChannelWithLengthCounter::reset(bool soft) {
    ApuBaseChannel::reset(soft);

    enabled = false;
    if (channel_type == ApuChannelType::TRIANGLE || !soft) {
        halt = false;
        reload_halt = false;

        length_counter = 0;
        reload_length_counter = 0;
        prev_length_counter = 0;
    }
}

void ApuChannelWithLengthCounter::clock_length_counter() {
    if (!halt && length_counter > 0) {
        length_counter--;
    }
}

void ApuChannelWithLengthCounter::set_length_counter(uint8_t value) {
    if (enabled) {
        length_counter = length_table[value];
        prev_length_counter = length_counter;
        // TODO maybe say something to APU?
    }
}

void ApuChannelWithLengthCounter::write_halt(bool new_halt) {
    reload_halt = new_halt;
}

void ApuChannelWithLengthCounter::write_enabled(bool new_enabled) {
    if (!new_enabled) {
        reset(true);
    }
    enabled = new_enabled
}
