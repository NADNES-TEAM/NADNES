#include "apu.h"
#include "apu_container.h"
#include "apu_dmc.h"
#include "apu_noise.h"
#include "apu_pulse.h"
#include "apu_triangle.h"
#include "nes_devices/R6502.h"
#include "apu_frame_counter.h"
#include "apu_envelope.h"

void Apu::run() {
    uint32_t cycles_to_run = current_cycle - previous_cycle;
    while (cycles_to_run > 0) {
        auto res = frame_counter->run_cycles(cycles_to_run);
        cycles_to_run -= res.actually_ran;
        pulse_channel[0]->reload_counter();
        pulse_channel[1]->reload_counter();
        noise_channel->reload_counter();
        triangle_channel->reload_counter();

        pulse_channel[0]->run_till((int)previous_cycle);
        pulse_channel[1]->run_till((int)previous_cycle);
        noise_channel->run_till((int)previous_cycle);
        triangle_channel->run_till((int)previous_cycle);
        dmc_channel->run_till((int)previous_cycle);
    }
}

void Apu::set_need_to_run() {}

Apu::Apu(NES::Cpu *cpu) {
    apu_enabled = true;
    is_need_to_run = false;

    apu_container = std::make_unique<ApuContainer>(this, mixer.get(), cpu);
    pulse_channel[0] = std::make_unique<ApuPulse>(ApuChannelType::PULSE1, apu_container.get());
    pulse_channel[1] = std::make_unique<ApuPulse>(ApuChannelType::PULSE2, apu_container.get());
    triangle_channel = std::make_unique<ApuTriangle>(ApuChannelType::TRIANGLE, apu_container.get());
    noise_channel = std::make_unique<ApuNoise>(ApuChannelType::NOISE, apu_container.get());
    dmc_channel = std::make_unique<ApuDmc>(ApuChannelType::DMC, apu_container.get());

    reset(false);
}

void Apu::CPU_write(std::uint16_t addr, std::uint8_t value) {
    assert(addr == 0x4015);
    run();
//    apu_container->get_cpu()
    // TODO clear irq

    pulse_channel[0]->write_enabled(bool(value & 1));
    pulse_channel[1]->write_enabled(bool(value & 2));
    triangle_channel->write_enabled(bool(value & 4));
    noise_channel->write_enabled(bool(value & 8));
    // TODO DMC
}

bool Apu::need_to_run(uint32_t cur_cycle) {
    if (dmc_channel->need_to_run() || is_need_to_run) {
        is_need_to_run = false;
        return true;
    }
    uint32_t cycles_to_run = cur_cycle - previous_cycle;
    return frame_counter->need_to_run (cycles_to_run) || dmc_channel->irq_pending(cycles_to_run);
}

void Apu::clock_frame_counter(Frame type) {
    pulse_channel[0]->clock_envelope();
    pulse_channel[1]->clock_envelope();
    triangle_channel->clock_linear_counter();
    noise_channel->clock_envelope();

    if (type == Frame::HALF) {
        //Half frames clock length counter & sweep
        pulse_channel[0]->clock_length_counter();
        pulse_channel[1]->clock_length_counter();
        triangle_channel->clock_length_counter();
        noise_channel->clock_length_counter();

        pulse_channel[0]->clock_sweep();
        pulse_channel[1]->clock_sweep();
    }
}

void Apu::reset(bool soft) {
    apu_enabled = true;
    current_cycle = 0;
    previous_cycle = 0;
    pulse_channel[0]->reset(soft);
    pulse_channel[1]->reset(soft);
    triangle_channel->reset(soft);
    noise_channel->reset(soft);
    dmc_channel->reset(soft);
    frame_counter->reset(soft);
}


void Apu::exec() {
    current_cycle++;
    if(current_cycle == mixer::CycleLength - 1) {
        end_frame();
    } else if(need_to_run(current_cycle)) {
        run();
    }
}

void Apu::process_cpu_clock(){
    if(apu_enabled) {
        exec();
    }
}

void Apu::end_frame() {
    run();
    pulse_channel[0]->end_frame();
    pulse_channel[1]->end_frame();
    triangle_channel->end_frame();
    noise_channel->end_frame();
    dmc_channel->end_frame();

    mixer->PlayAudioBuffer(_currentCycle);

    current_cycle = 0;
    previous_cycle = 0;
}

bool Apu::is_apu_enabled() {
    return apu_enabled;
}
