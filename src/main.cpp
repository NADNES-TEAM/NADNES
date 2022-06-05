#include <QApplication>
#include <filesystem>
#include "main_controller.h"

int main(int argc, char *argv[]) {
#ifdef __APPLE__
    std::filesystem::path cur(argv[0]);
    std::filesystem::current_path(cur.parent_path());
#endif

    QApplication application(argc, argv);
    MainController controller;
    return QApplication::exec();
}
