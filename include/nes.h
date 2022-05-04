#pragma once

#include <QObject>
#include <string>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"
#include "nes_devices/R6502.h"
#include "nes_devices/bus.h"
#include "nes_devices/cartridge.h"
#include "nes_devices/controller.h"
#include "nes_devices/ppu.h"

namespace NES {
struct Nes {
public:
    Nes(const std::string &filename, ScreenInterface *screen_, KeyboardInterface *keyboard_);

    void tick();

    void reset();

    void save(const std::string &filename);

    void load(const std::string &filename);

private:
    Cartridge cartridge;
    Ppu ppu;
    Cpu cpu;
    Bus bus;
    Controller controller;
};
}  // namespace NES
