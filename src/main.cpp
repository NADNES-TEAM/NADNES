
#include <QApplication>
#include <QTimer>
#include "Emulator.h"


int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    NES::Emulator emulator;
    auto *timer = new QTimer(&application);
    QObject::connect(timer, &QTimer::timeout,
                     &emulator, &NES::Emulator::clock);

    return QApplication::exec();
}

