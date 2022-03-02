#include <QObject>
#include "screen.h"
#pragma once

namespace NES {

class Emulator : public QObject {
    Q_OBJECT
public:
    explicit Emulator(QObject *parent = nullptr);
    ~Emulator() override = default;

signals:

public slots:
    void clock();
private:
    unsigned int m_counter{};
    NES::Screen m_screen;
};

}