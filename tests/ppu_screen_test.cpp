#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <chrono>
#include <thread>
#include "nes.h"
#include "screen.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    NES::Screen screen;
    NES::Nes nes("../SMB.nes", &screen);
    auto *timer = new QTimer(&application);
    QObject::connect(timer, &QTimer::timeout, &nes, &NES::Nes::tick, Qt::DirectConnection);
    timer->start(std::chrono::milliseconds(1000 / 60));
    return QApplication::exec();
}