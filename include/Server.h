#pragma once

#include <QDialog>
#include <QLabel>
#include <QTcpServer>
#include "environment/player_select_window.h"
#include "remote_player.h"

class Server : public QDialog {
public:
    explicit Server(PlayerManager *player_manager, QWidget *parent = nullptr);

    void open_connections() const;

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
    PlayerManager *m_player_manager = nullptr;

    std::atomic<int> m_next_id{2};
};
