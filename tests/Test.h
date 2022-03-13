#pragma once

#include <Controller.h>
#include <QLabel>
#include <QObject>
#include <QWidget>
#include "KeyboardInterface.h"
#include "ScreenInterface.h"

namespace NES {

class Test : public QObject {
    Q_OBJECT
public:
    explicit Test(KeyboardInterface *keyboardInterface);

public slots:
    void clock();

private:
    QLabel *label;
    std::unique_ptr<Controller> controller;
    ScreenInterface *screenInterface{};
};

}  // namespace NES
