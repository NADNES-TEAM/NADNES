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
#include "search.h"
namespace NES {
struct Nes {
public:
    Nes(const std::string &filename, ScreenInterface *screen_, KeyboardInterface *keyboard_);

    void tick();

    void reset();

    std::vector<Search::ResultRaw> search(const Search::ParamsOfSearch& params);
    bool change_memory_value(const Search::ParamsOfChange& params);
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
