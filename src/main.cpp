#include <QApplication>
#include <QTimer>
#include <chrono>
#include <iostream>
#include "environment/main_window.h"
#include "nes.h"
#include "nes_properties.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    NES::MainWindow main_window;
    main_window.show();
    auto *keyboardInterface = main_window.get_keyboard_interface();
    auto *screenInterface = main_window.get_screen_interface();
    NES::Nes nes("../DK.nes", screenInterface, keyboardInterface);
    auto *timer = new QTimer(&application);
    timer->callOnTimeout([&nes]() {
        nes.tick();
    });
    // 17 ms = (58.8235 Hz) ^ -1, ideal is 16.6393 ms = (60.0988 Hz) ^ -1
    // APU_FRAME_COUNTER_FREQ_HZ = 60
    // PPU_VERTICAL_FRAME_RATE_FREQ_HZ = 60.0988
    timer->start(std::chrono::milliseconds(lround(1000.0 / NES::PPU_VERTICAL_FRAME_RATE_FREQ_HZ)));
    return QApplication::exec();
}
