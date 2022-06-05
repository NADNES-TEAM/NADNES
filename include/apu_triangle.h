#pragma once

#include "apu_fwd.h"
#include "apu_length_counter.h"

class ApuTriangle : public ApuLengthCounter {
public:
    ApuTriangle(ApuChannelType channel_type, ApuContainer *apu_container);

    void clock() override;

    void reset(bool soft) override;

    void CPU_write(uint16_t addr, uint8_t value);

    void clock_linear_counter();

private:
    static const std::array<uint8_t, 32> seq;

    uint8_t linear_counter{};
    uint8_t linear_counter_reload{};
    bool linear_reload_flag{};
    bool linear_control_flag{};
    uint8_t seq_pos{};
};

