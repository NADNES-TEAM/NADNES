#include <QApplication>
#include <QPushButton>
#include "../tests/Test.h"
#include <QTimer>
#include <QObject>
#include <QLabel>

int main(int argc, char *argv[]) {
    // Z: A
    // X: B
    // Select: Enter
    // Start: Space
    // Up: Arrow up
    // Down: Arrow down
    // Left: Arrow left
    // Right: Arrow right
    QApplication application(argc, argv);

    auto *test = new NES::Test();
    test->show();
    auto *timer = new QTimer(&application);
    QObject::connect(timer, SIGNAL(timeout()), test, SLOT(clock()));
    timer->start(std::chrono::milliseconds(200));

    return QApplication::exec();
}
