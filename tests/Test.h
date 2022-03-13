#pragma once

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <Controller.h>
#include "KeyboardInterface.h"

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
};

}