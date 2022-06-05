#pragma once

#include "apu_enums.h"
#include "apu_envelope.h"
#include "apu_fwd.h"

class ApuNoise : public ApuEnvelope {
public:
    ApuNoise(ApuChannelType channel_type, ApuContainer *apu_container);

    [[nodiscard]] bool is_muted() const;

    void clock() override;

    void reset(bool soft) override;

    void CPU_write(uint16_t addr, uint8_t value);

private:
    static const std::array<uint16_t, 16> noise_table;

    uint16_t shift_reg = 1;  // !!!
    bool noise_mode;
};