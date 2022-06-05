#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include "apu_fwd.h"
#include "apu_enums.h"

class Apu {
public:
    explicit Apu(NES::Cpu *cpu);

    void CPU_write(std::uint16_t addr, std::uint8_t value);

    uint32_t get_clock_count();

    void set_need_to_run();

    void run();

    void reset(bool soft);

    void exec();

    void process_cpu_clock();

    void end_frame();

private:
    bool is_need_to_run{};
    bool apu_enabled{};

    uint32_t previous_cycle{};
    uint32_t current_cycle{};

    std::unique_ptr<ApuContainer> apu_container{};
    ApuChannelType channel_type{};

    std::unique_ptr<ApuPulse> pulse_channel[2];
    std::unique_ptr<ApuTriangle> triangle_channel;
    std::unique_ptr<ApuNoise> noise_channel;
    std::unique_ptr<ApuDmc> dmc_channel;  // TODO
    std::unique_ptr<ApuFrameCounter> frame_counter;

    std::unique_ptr<ApuMixer> mixer;

    bool need_to_run(uint32_t cur_cycle);

    void FrameCounterTick(Frame type);
};
