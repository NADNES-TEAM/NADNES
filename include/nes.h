#pragma once

#include <QObject>
#include <chrono>
#include <string>
#include "PPU.h"
#include "bus.h"
#include "cartridge.h"
#include "mos6502.h"
#include "screen.h"

namespace NES {
struct Nes : public QObject {
    Q_OBJECT
public:
    Nes(const std::string &filename, ScreenInterface *screen_);
    ~Nes() override = default;

signals:

public slots:
    void tick();

private:
    Cartridge cartridge;
    Ppu ppu;
    ScreenInterface *screen;
    mos6502 cpu;
    Bus bus;
    uint64_t cpu_cycle_counter = 1;
};
}  // namespace NES