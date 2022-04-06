#pragma once
#include <cstdint>
#include "all_nes_fwd.h"
#include "connect_token.h"

namespace NES {

class Dma {
    uint8_t start_address = 0;
    int counter = 0;
    bool activated = false;
    bool started = false;
    Bus *bus = nullptr;
    uint8_t transfer_data = 0;

public:
    void connect(Bus *bus_, ConnectToken) noexcept;
    void tick(bool even_cycle);
    [[nodiscard]] bool is_active() const noexcept;
    void activate(uint8_t address);
    void reset();
};

}  // namespace NES
