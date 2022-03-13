#include "nes.h"


namespace NES {

void Nes::tick() {
    bool frame_rendered = false;
    int64_t frame_counter = 0;
    while (!frame_rendered) {
        if (frame_counter % 3 == 0) {
            cpu.tick();
        }
        frame_rendered = ppu.tick();
        frame_counter += 1;
    };
}

Nes::Nes(const std::string &filename, ScreenInterface *screen_, KeyboardInterface *keyboard_) : cartridge(filename){
    ppu.connect(screen_, ConnectToken());
    ppu.connect(&cartridge, ConnectToken());
    ppu.connect(&cpu, ConnectToken());

    bus.connect(&ppu, ConnectToken());
    bus.connect(&cartridge, ConnectToken());
    bus.connect(&controller, ConnectToken());

    controller.connect(keyboard_, ConnectToken());

    cpu.connect(&bus, ConnectToken());
    cpu.reset();
}

}  // namespace NES