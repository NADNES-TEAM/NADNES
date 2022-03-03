#include "R6502.h"
#include "bus.h"
#include "mapper.h"
#include "PPU.h"
#include "screen.h"

#include <QApplication>
#include <QPushButton>


int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    NES::Screen screen;
    screen.refresh_screen();

    return QApplication::exec();
}

