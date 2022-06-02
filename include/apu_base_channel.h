#pragma once

#include <cstdint>
#include "apu_enums.h"
#include "apu_fwd.h"

class ApuBaseChannel {
public:
    virtual void clock() = 0;
    ApuBaseChannel(ApuContainer *apu_container);
    virtual void reset(bool soft_reset);
    void run_till(int new_cycle);
    void end_frame();
    void add_output();

protected:
    uint8_t output;
    uint16_t timer;
    uint16_t period;

private:
    int previous_cycle;
    ApuChannelType channel_type;
    ApuContainer *apu_container{};
};
