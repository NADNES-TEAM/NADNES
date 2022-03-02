#include "Emulator.h"
#include <thread>
#include <iostream>

namespace NES {

Emulator::Emulator(QObject *parent) : QObject(parent) {
}
void Emulator::clock() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    if (m_counter++ % 4 != 0) {
        return;  // Only PPU
    }
    static int k = 0;
    if (k > 23) {
        return;
    }
    for (int i = k * 10; i < (k + 1) * 10; ++i) {
        for (int j = k * 10; j < (k + 1) * 10; ++j) {
            m_screen.set_pixel(i, j, 0b110000);
        }
    }
    k++;
    m_screen.refresh_screen();
}

}  // namespace NES