#pragma once

#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include "nes.h"
#include <interfaces/screen_interface.h>
#include "environment/connection_window.h"

struct RemoteEmulator: public QObject {
    Q_OBJECT

public:
    RemoteEmulator(QObject *parent, NES::ScreenInterface *screen_);
    ~RemoteEmulator() override =default;
    void close();

public slots:
    void key_changed(uint8_t btn);
    void show_connection_window();
    void handle_error(QAbstractSocket::SocketError error);

private:
    ConnectionWindow connection_window;
    QDataStream stream;
    QTcpSocket *socket;
    NES::ScreenInterface *screen;
    uint8_t cur_x, cur_y;

private slots:
    void try_connect(const QString &address, int port);
    void data_arrived();

};
