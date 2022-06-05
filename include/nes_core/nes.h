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
#include "cheating/search.h"

namespace NES {
struct Nes {
public:
    Nes(const std::string &filename,
        ScreenInterface *screen_,
        KeyboardInterface *keyboard_1_,
        KeyboardInterface *keyboard_2_);

    void tick();

    void reset();

    std::vector<Cheating::ResultRaw> search(
        const Cheating::ParamsOfSearch &params,
        const std::vector<Cheating::ResultRaw> &old_result_data);
    static bool change_memory_value(const Cheating::ParamsOfChange &params);
    void save(const std::string &filename);

    [[nodiscard]] uint64_t get_hash() const;

    void load(const std::string &filename);

private:
    Cartridge cartridge;
    Ppu ppu;
    Cpu cpu;
    Bus bus;
    Controller controller;
};
}  // namespace NES
