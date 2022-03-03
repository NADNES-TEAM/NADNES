#include "nes.h"

namespace NES {

void Nes::tick() {
    bool frame_rendered = false;
    int64_t frame_counter = 0;
    while (!frame_rendered) {
        if (frame_counter % 3 == 0) {
            cpu.clock();
        }
        frame_rendered = ppu.tick();
        frame_counter += 1;
    };
}

}  // namespace NES