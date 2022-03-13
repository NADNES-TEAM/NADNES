#include <QApplication>
#include <QPushButton>
#include "../tests/Test.h"
#include <QTimer>
#include <QObject>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // Z: A
    // X: B
    // Select: Tab
    // Start: Enter
    // Up: Arrow up
    // Down: Arrow down
    // Left: Arrow left
    // Right: Arrow right
    QApplication application(argc, argv);


    NES::MainWindow mainWindow;
    mainWindow.show();
    auto test = new NES::Test(dynamic_cast<KeyboardInterface *>(&mainWindow));
    auto *timer = new QTimer(&application);
    QObject::connect(timer, SIGNAL(timeout()), test, SLOT(clock()));
    timer->start(std::chrono::milliseconds(20));

//    return QApplication::exec();
//    QApplication application(argc, argv);
//
//    NES::MainWindow mainWindow;
//    mainWindow.show();
//
//    return QApplication::exec();
}
