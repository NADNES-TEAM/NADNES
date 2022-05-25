#include <QApplication>
#include "main_controller.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainController controller;
    return QApplication::exec();
}
