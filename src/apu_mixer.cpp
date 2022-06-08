#include "apu_mixer.h"
#include <algorithm>

ApuMixer::ApuMixer(ApuContainer *apu_container) : apu_container(apu_container) {

}

void ApuMixer::add_delta(ApuChannelType channel_type, int time, int16_t delta) {
    if (delta != 0) {
        timestamps.push_back(time);
        channel_output[(int)channel_type][time] += delta;
    }
}

void ApuMixer::end_frame(int time) {
    double master_volume = 1.0;
    double fade_ratio = 1.0;
    std::sort(timestamps.begin(), timestamps.end());
    timestamps.erase(std::unique(timestamps.begin(), timestamps.end()), timestamps.end());
    bool mute_frame = true;
    for (int i = 0, len = timestamps.size(); i < len; ++i) {
        uint32_t stamp = timestamps[i];
        for (int j = 0; j < 5; ++j) {
            if (channel_output[j][stamp] != 0) {
                mute_frame = false;
            }
            cur_output[j] += channel_output[j][stamp];
        }

        int16_t cur_volume;
    }

}



double ApuMixer::get_output_volume() {
    double square_output = get_channel_output(ApuChannelType::PULSE1)
                        + get_channel_output(ApuChannelType::PULSE2);
    double tnd_output = 3 * get_channel_output(ApuChannelType::TRIANGLE)
                     + 2 * get_channel_output(ApuChannelType::NOISE)
                     + get_channel_output(ApuChannelType::DMC);

    uint16_t square_volume = (uint16_t)(477600 / (8128.0 / square_output + 100.0));
    uint16_t tnd_volume = (uint16_t)(818350 / (24329.0 / tnd_output + 100.0));

    return square_volume + tnd_volume;
}

double ApuMixer::get_channel_output(ApuChannelType type) {
    return cur_output[(int)type];
}
