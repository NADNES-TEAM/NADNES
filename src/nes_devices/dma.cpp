#include "nes_devices/dma.h"
#include "nes_devices/bus.h"

namespace NES {
void Dma::activate(uint8_t address) {
    start_address = address;
    counter = 0;
    activated = true;
}

void Dma::connect(Bus *bus_, ConnectToken) noexcept {
    bus = bus_;
}

bool Dma::is_active() const noexcept {
    return activated;
}

void Dma::tick(bool even_cycle) {
    if (!started && !even_cycle) {
        return;
    }
    started = true;
    if (even_cycle) {
        transfer_data = bus->mem_read(start_address * 0x100 + counter++);
    } else {
        bus->mem_write(0x2004, transfer_data);
    }
    if (counter == 256) {
        started = false;
        activated = false;
        counter = 0;
    }
}

void Dma::reset() {
    activated = false;
}

}  // namespace NES
