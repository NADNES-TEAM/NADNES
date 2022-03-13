#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <thread>
#include "nes.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    try {
        QApplication application(argc, argv);
        NES::MainWindow mainWindow;
        mainWindow.show();
        auto *keyboardInterface = mainWindow.getKeyboardInterface();
        auto *screenInterface = mainWindow.getScreenInterface();
        NES::Nes nes("../nestest.nes", screenInterface, keyboardInterface);
        auto *timer = new QTimer(&application);
        QObject::connect(timer, &QTimer::timeout, &nes, &NES::Nes::tick, Qt::DirectConnection);
        timer->start(std::chrono::milliseconds(1000 / 60));
        return QApplication::exec();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
