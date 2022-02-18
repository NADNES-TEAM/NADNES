#ifndef NADNES_APU_H
#define NADNES_APU_H

#include <cstdint>

namespace NADNES {

class Apu {
public:
    Apu();
    ~Apu();

    union {
        struct {
            std::uint8_t:  first_pulse
        };
    };

    private:
};

}

#endif  // NADNES_APU_H
