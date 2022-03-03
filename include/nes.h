#pragma once

#include <QObject>
#include <chrono>
#include <string>
#include "PPU.h"
#include "R6502.h"
#include "bus.h"
#include "cartridge.h"
#include "connect_token.h"
#include "screen.h"

namespace NES {
struct Nes : public QObject {
    Q_OBJECT
public:
    Nes(const std::string &filename, ScreenInterface *screen_) : cartridge(filename) {
        screen = screen_;
        ppu.connect(screen, ConnectToken());
        ppu.connect(&cartridge, ConnectToken());
        cpu.connect(&bus, ConnectToken());
        bus.connect(&ppu, ConnectToken());
        bus.connect(&cartridge, ConnectToken());
    }
    ~Nes() override = default;

signals:

public slots:
    void tick();

private:
    Cartridge cartridge;
    Ppu ppu;
    ScreenInterface *screen;
    CPU cpu;
    Bus bus;
};
}  // namespace NES