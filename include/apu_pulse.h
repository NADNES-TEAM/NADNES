#pragma once

#include "apu_envelope.h"

class ApuPulse : public ApuEnvelope {
public:
    ApuPulse(ApuChannelType channel_type, ApuContainer *apu_container);

    void sweep_write(uint8_t value);

    void update_target_period();

    void set_period(uint16_t new_period);

    [[nodiscard]] bool is_muted() const;

    void update_output();
    void clock() override;
    void reset(bool soft) override;

    void CPU_write(uint16_t addr, uint8_t value);

    void clock_sweep();

protected:

    bool sweep_enabled{};
    uint8_t sweep_period{}; // divider's period is sweep_period + 1 halves
    bool sweep_negate{};
    uint8_t sweep_shift{};  // number of bits for a shift
    bool sweep_reload{};
    uint8_t sweep_divider{};
    uint32_t sweep_target_period{};
    uint16_t real_period{};

    uint8_t duty{};  // index in duty_seq
    uint8_t duty_pos{};  // bit in duty_seq[duty]

    static const std::array<uint8_t, 4> duty_seq;
};