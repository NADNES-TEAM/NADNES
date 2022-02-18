#ifndef NADNES_APU_H
#define NADNES_APU_H

#include <cstdint>
#include <cassert>

namespace NADNES {

struct Timer {
    //
};

class Apu {
public:
    Apu();
    ~Apu();

    void write(std::uint16_t addr, std::uint8_t value) {

    }
private:
    struct Pulse {

    };
};

}

#endif  // NADNES_APU_H
