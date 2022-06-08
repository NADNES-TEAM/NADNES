#pragma once

#include <cstdint>
#include <vector>
#include "apu_fwd.h"
#include "apu_enums.h"

class ApuMixer {
public:
    static constexpr uint32_t CYCLE_LENGTH = 10000;
    explicit ApuMixer(ApuContainer *apu_container);

    void add_delta(ApuChannelType channel_type, int time, int16_t delta);

    double get_channel_output(ApuChannelType type);

    double get_output_volume();

    void end_frame(int time);

    void reset();

private:
    ApuContainer *apu_container{};
    uint16_t previous_output;
    std::vector<int16_t> timestamps;
    int16_t channel_output[5][CYCLE_LENGTH];
    int16_t cur_output[5];
};
