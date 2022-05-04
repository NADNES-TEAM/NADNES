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
    return QApplication::exec();
}
