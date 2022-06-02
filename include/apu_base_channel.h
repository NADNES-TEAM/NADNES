#pragma once

#include <cstdint>
#include "apu_enums.h"
#include "apu_fwd.h"

class ApuBaseChannel {
public:
    virtual void clock() = 0;
    ApuBaseChannel(ApuChannelType channel_type, ApuContainer *apu_container);
    virtual void reset(bool soft_reset);
    void run_till(int new_cycle);
    void end_frame();
    void add_output(uint8_t new_output);

protected:
    uint8_t prev_output;
    uint16_t timer;
    uint16_t period;
    ApuChannelType channel_type;
    int prev_cycle;
    ApuContainer *apu_container{};
};
