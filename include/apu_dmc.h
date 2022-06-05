#pragma

#include <array>
#include "apu_fwd.h"
#include "apu_base_channel.h"

// TODO !!!
class ApuDmc : public ApuBaseChannel {
public:
    ApuDmc(ApuChannelType type, ApuContainer *apu_container);

private:
    static const std::array<uint16_t, 16> period_table;

    uint16_t sample_addr{};
    uint16_t sample_len{};
    uint8_t output_lvl{};
    bool irq_enabled{};
    bool loop_flag{};

    uint16_t cur_addr{};
    uint16_t bytes_remaining{};
    uint8_t read_buffer{};
    bool buffer_empty{true};

    uint8_t shift_reg{};
    uint8_t bits_remaining{};
    bool silence_flag{true};
    bool need_run{};
    bool need_init{};

    uint8_t last_value_4011{};

    void init_sample();

    void clock() override;

public:
    void reset(bool soft) override;

    void set_enabled(bool enabled);

    bool need_to_run();

    bool irq_pending(uint32_t cycles_to_run);
};