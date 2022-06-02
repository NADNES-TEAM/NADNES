#pragma once

#include <cstdint>
#include "apu_fwd.h"
#include "apu_enums.h"

class ApuMixer {
public:
    explicit ApuMixer(ApuContainer *apu_container);
    void add_delta(ApuChannelType channel_type, int time, uint16_t delta);
    void reset();

private:
    ApuContainer *apu_container{};
};
