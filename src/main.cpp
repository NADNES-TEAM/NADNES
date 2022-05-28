#include <QApplication>
#include "main_controller.h"

int main(int argc, char *argv[]) {
    QGuiApplication application(argc, argv);
    MainController controller;
    return QGuiApplication::exec();
}
