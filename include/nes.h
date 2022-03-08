#pragma once

#include <QObject>
#include <chrono>
#include <string>
#include "PPU.h"
#include "../correct_cpu_for_testing/mos6502.h"
#include "bus.h"
#include "cartridge.h"
#include "connect_token.h"
#include "screen.h"

namespace NES {
struct Nes : public QObject {
    Q_OBJECT
public:
    Nes(const std::string &filename, ScreenInterface *screen_) : cartridge(filename), cpu(nullptr) {
        screen = screen_;
        ppu.connect(screen, ConnectToken());
        ppu.connect(&cartridge, ConnectToken());
        bus.connect(&ppu, ConnectToken());
        bus.connect(&cartridge, ConnectToken());
        cpu = mos6502(&bus);
        cpu.Reset();
    }
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