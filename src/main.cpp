#include <QApplication>
#include "environment/main_window.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    NES::MainWindow main_window;
    main_window.show();
    return QApplication::exec();
}
