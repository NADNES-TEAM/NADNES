#include "Test.h"
#include <QWidget>

namespace NES {

Test::Test(QWidget *parent) : QWidget(parent) {
    controller = new Controller();
    label = new QLabel();
}
void Test::clock() {
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