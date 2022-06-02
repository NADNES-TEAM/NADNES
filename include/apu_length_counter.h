#pragma once

#include <array>
#include "apu_fwd.h"
#include "apu_enums.h"
#include "apu_container.h"
#include "apu_base_channel.h"

class ApuChannelWithLengthCounter : public ApuBaseChannel {
public:
    ApuChannelWithLengthCounter(ApuChannelType channel_type, ApuContainer *apu_container);

    void reset(bool soft) override;

    void clock_length_counter();

    void set_length_counter(uint8_t value);

    void write_enabled(bool new_enabled);

    void write_halt(bool new_halt);

private:
    static const std::array<uint8_t, 32> length_table;
    bool enabled;
    bool halt;
    bool reload_halt;
    uint8_t length_counter;
    uint8_t reload_length_counter;
    uint8_t prev_length_counter;

};