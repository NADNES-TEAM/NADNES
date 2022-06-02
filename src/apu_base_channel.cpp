#include "apu_base_channel.h"
#include "apu_container.h"
#include "apu_mixer.h"

ApuBaseChannel::ApuBaseChannel(ApuChannelType channel_type, ApuContainer *apu_container)
    : apu_container(apu_container), channel_type(channel_type) {
    ApuBaseChannel::reset(false);
}

void ApuBaseChannel::reset(bool soft_reset) {
    timer = 0;
    period = 0;
    prev_output = 0;
    prev_cycle = 0;
    apu_container->get_mixer()->reset();
}

void ApuBaseChannel::run_till(int new_cycle) {
    int diff_cycles = new_cycle - prev_cycle;
    // timer is clocked: timer, timer - 1, timer - 2 ... 0
    // so "period" of timer in normal meaning is actually (timer + 1)
    while (diff_cycles >= timer + 1) {
        diff_cycles -= timer + 1;
        prev_cycle += timer + 1;
        clock();
        timer = period;
    }

    timer -= diff_cycles;
    prev_cycle = new_cycle;
}

void ApuBaseChannel::end_frame() {
    prev_cycle = 0;
}

void ApuBaseChannel::add_output(uint8_t new_output) {
    // prev_cycle ... new_cycle
    // prev_output ... new output
    apu_container->get_mixer()->add_delta(channel_type, prev_cycle, new_output - prev_output);
    prev_output = new_output;
}
