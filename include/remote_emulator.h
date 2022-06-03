#pragma once

#include <interfaces/screen_interface.h>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "environment/connection_window.h"
#include "nes.h"
#include "nes_properties.h"
struct RemoteEmulator : public QObject {
    Q_OBJECT

public:
    RemoteEmulator(QObject *parent, NES::ScreenInterface *screen_);
    ~RemoteEmulator() override = default;
    void close();

public slots:
    void key_changed(uint8_t btn);
    void show_connection_window();
    void handle_error(QAbstractSocket::SocketError error);

private:
    void read_settings();
    void write_settings() const;
    void ask_for_reconnection(const QString &msg = "");
    QTimer timer;
    ConnectionWindow *connection_window;
    QTcpSocket *socket;
    NES::ScreenInterface *screen;
    QString last_address;
    int last_port = -1;

private slots:
    void try_connect(const QString &address, int port);
    void data_arrived();
};
