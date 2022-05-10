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
#include "search.h"

namespace NES {
struct Nes {
public:
    Nes(const std::string &filename,
        ScreenInterface *screen_,
        KeyboardInterface *keyboard_1_,
        KeyboardInterface *keyboard_2_);

    void tick();

    void reset();

    std::vector<Search::ResultRaw> search(const Search::ParamsOfSearch& params);
    bool change_memory_value(const Search::ParamsOfChange& params);
    void save(const std::string &filename);

    void load(const std::string &filename);

private:
    Cartridge cartridge;
    Ppu ppu;
    Cpu cpu;
    Bus bus;
    Controller controller;
    std::vector<Search::ResultRaw> result_data;
    void get_memory_p(uint8_t *&memory_p,size_t &memory_size,const Search::Params &params);
};
}  // namespace NES
