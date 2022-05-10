#include "nes.h"
#include <fstream>
#include "nes_exceptions.h"
#include "reset_token.h"

namespace NES {

void Nes::tick() {
    bool frame_rendered = false;
    int64_t frame_counter = 0;
    while (!frame_rendered) {
        if (frame_counter % 3 == 0) {
            cpu.tick(frame_counter % 2 == 0);
        }
        frame_rendered = ppu.tick();
        frame_counter += 1;
    };
}

Nes::Nes(const std::string &filename,
         ScreenInterface *screen_,
         KeyboardInterface *keyboard_1_,
         KeyboardInterface *keyboard_2_)
    : cartridge(filename) {
    ppu.connect(screen_, ConnectToken());
    ppu.connect(&cartridge, ConnectToken());
    ppu.connect(&cpu, ConnectToken());

    bus.connect(&ppu, ConnectToken());
    bus.connect(&cartridge, ConnectToken());
    bus.connect(&controller, ConnectToken());

    controller.connect_to_port1(keyboard_1_, ConnectToken());
    controller.connect_to_port2(keyboard_2_, ConnectToken());

    cpu.connect(&bus, ConnectToken());
    cpu.reset(ResetToken());
}

void Nes::reset() {
    ppu.reset(ResetToken());
    cartridge.reset(ResetToken());
    cpu.reset(ResetToken());
}

void Nes::save(const std::string &filename) {
    auto file = std::ofstream(filename, std::ios::binary);
    if (!file) {
        throw UnableToOpenFileError(filename);
    }
    cpu.save(file);
    ppu.save(file);
    cartridge.save(file);
    bus.save(file);
    file.close();
}

void Nes::load(const std::string &filename) {
    auto file = std::ifstream(filename, std::ios::binary);
    if (!file) {
        throw UnableToOpenFileError(filename);
    }
    cpu.load(file);
    ppu.load(file);
    cartridge.load(file);
    bus.load(file);
    file.close();
}

}  // namespace NES
