#pragma once

#include <QDialog>
#include <QLabel>
#include <QTcpServer>
#include "remote_player.h"

class Server : public QDialog {


public:
    explicit Server(QWidget *parent = nullptr);

public slots:

private:
    void init_server();

    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    std::vector<RemotePlayer> players;
};
