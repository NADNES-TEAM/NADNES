#include "Test.h"
#include <QWidget>
#include <random>
#include "ScreenInterface.h"

namespace NES {

Test::Test(KeyboardInterface *keyboardInterface) : QObject(nullptr) {
    screenInterface = dynamic_cast<ScreenInterface *>(keyboardInterface);
    controller = std::make_unique<Controller>(keyboardInterface);
    label = new QLabel();
}

void Test::clock() {
    static std::mt19937 gen;
    int row = std::uniform_int_distribution<int>(0, 100)(gen);
    int column = std::uniform_int_distribution<int>(0, 100)(gen);
    screenInterface->setPixel(row, column, Color{100, 100, 100});
    screenInterface->refreshScreen();
    controller->CpuWrite(0x4016, 1);
    QString s;
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    s.push_back(QChar('0' + controller->CpuRead(0x4016)));
    label->setText(s);
    label->show();
}
}  // namespace NES
