#include "nes.h"

namespace NES {

void Nes::tick() {
    bool frame_rendered = false;
    int64_t frame_counter = 0;
    while (!frame_rendered) {
        if (frame_counter % 3 == 0 && --cpu_cycle_counter == 0) {
            cpu.Run(1, cpu_cycle_counter);
        }
        frame_rendered = ppu.tick();
        frame_counter += 1;
    };
}

Nes::Nes(const string &filename, ScreenInterface *screen_) : cartridge(filename), cpu(nullptr) {
    screen = screen_;
    ppu.connect(screen, ConnectToken());
    ppu.connect(&cartridge, ConnectToken());
    bus.connect(&ppu, ConnectToken());
    bus.connect(&cartridge, ConnectToken());
    cpu = mos6502(&bus);
    ppu.connect(&cpu, ConnectToken());
    cpu.Reset();
}

}  // namespace NES