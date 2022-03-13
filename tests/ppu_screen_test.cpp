#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <thread>
#include "nes.h"
#include "ScreenInterface.h"
#include "MainWindow.h"
#include "nes_exceptions.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    NES::MainWindow mainWindow;
    mainWindow.show();
    auto *keyboardInterface = mainWindow.getKeyboardInterface();
    auto *screenInterface = mainWindow.getScreenInterface();
    NES::Nes nes("C:\\Users\\milae\\Trash_Downloads\\nestest.nes", screenInterface);
    auto *timer = new QTimer(&application);
    QObject::connect(timer, &QTimer::timeout, &nes, &NES::Nes::tick, Qt::DirectConnection);
    timer->start(std::chrono::milliseconds(1000 / 60));
    return QApplication::exec();
}