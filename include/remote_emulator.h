#pragma once
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include "nes.h"
#include <interfaces/screen_interface.h>
struct RemoteEmulator: public QObject {
public:
    RemoteEmulator(QTcpSocket *socket_, NES::ScreenInterface *screen_);
public slots:
    void key_changed(uint8_t btn);
private:
    QDataStream stream;
    QTcpSocket *socket;
    NES::ScreenInterface *screen;
    uint8_t cur_x,cur_y;
private slots:
    void data_arrived();
};
