#include <QApplication>
#include "main_controller.h"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
#ifdef __APPLE__
    std::filesystem::path cur(argv[0]);
    std::filesystem::current_path(cur.parent_path());
//    std::cout << "test"  << std::filesystem::current_path() << std::endl;
#endif

    QApplication application(argc, argv);
    MainController controller;
    return QApplication::exec();
}
