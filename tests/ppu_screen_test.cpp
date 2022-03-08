#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <thread>
#include "nes.h"
#include "nes_exceptions.h"
#include "screen.h"

int main(int argc, char *argv[]) {
    try {
        QApplication application(argc, argv);

        NES::Screen screen;
        NES::Nes nes("../nestest.nes", &screen);
        auto *timer = new QTimer(&application);
        QObject::connect(timer, &QTimer::timeout, &nes, &NES::Nes::tick, Qt::DirectConnection);
        timer->start(std::chrono::milliseconds(1000 / 60));
        return QApplication::exec();
    } catch (const std::exception &e) {
        std::cout << e.what();
    }
}