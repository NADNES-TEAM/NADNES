#pragma once

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <Controller.h>

namespace NES {

class Test : public QWidget {
    Q_OBJECT
public:
    explicit Test(QWidget *parent = nullptr);

public slots:
    void clock();

private:
    QLabel *label;
    Controller *controller;
};

}