#pragma once

#include <QObject>
#include <string>
#include "nes_devices/controller.h"
#include "interfaces/keyboard_interface.h"
#include "nes_devices/ppu.h"
#include "nes_devices/R6502.h"
#include "interfaces/screen_interface.h"
#include "nes_devices/bus.h"
#include "nes_devices/cartridge.h"

namespace NES {
struct Nes {
public:
    Nes(const std::string &filename, ScreenInterface *screen_, KeyboardInterface *keyboard_);

    void tick();

    void reset();

private:
    Cartridge cartridge;
    Ppu ppu;
    Cpu cpu;
    Bus bus;
    Controller controller;
};
}  // namespace NES
