#pragma once

#include <QDialog>
#include <QLabel>
#include <QTcpServer>
#include "remote_player.h"

class Server : public QDialog {
public:
    explicit Server(QWidget *parent = nullptr);

    void open_connections();

public slots:
    void init_server();
    void destruct_server();
    void on_shutdown_clicked();
    void on_run_clicked();
    void new_connection();

    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QPushButton *shutdownButton = nullptr;
    QPushButton *hideButton = nullptr;
    QPushButton *runButton = nullptr;

    std::atomic<int> nextId{2};
};
