#include "apu_base_channel.h"
#include "apu_container.h"
#include "apu_mixer.h"

ApuBaseChannel::ApuBaseChannel(ApuContainer *apu_container) : apu_container(apu_container) {
    ApuBaseChannel::reset(false);
}

void ApuBaseChannel::reset(bool soft_reset) {
    timer = 0;
    period = 0;
    output = 0;
    previous_cycle = 0;
    apu_container->get_mixer()->reset();
}

void ApuBaseChannel::run_till(int new_cycle) {
    int diff_cycles = new_cycle - previous_cycle;
    // timer is clocked: timer, timer - 1, timer - 2 ... 0
    // so "period" of timer in normal meaning is actually (timer + 1)
    while (diff_cycles >= timer + 1) {
        diff_cycles -= timer + 1;
        previous_cycle += timer + 1;
        clock();
        timer = period;
    }

    timer -= diff_cycles;
    previous_cycle = new_cycle;
}

void ApuBaseChannel::end_frame() {
    previous_cycle = 0;
}

void ApuBaseChannel::add_output() {

}
