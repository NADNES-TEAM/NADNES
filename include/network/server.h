#pragma once

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QTcpServer>
#include <QVBoxLayout>
#include "widgets/player_manager_window.h"
#include "settings/nes_config.h"
#include "network/remote_player.h"

class Server : public QDialog {
public:
    explicit Server(PlayerManager *player_manager_, QWidget *parent = nullptr);

    void make_connections() const;

public slots:
    void init_server();
    void destruct_server() const;
    void on_shutdown_clicked();
    void on_run_clicked();
    void new_connection();

private:
    static const int START_ID = 2;

    QLabel *statusLabel;
    QLabel *portLabel;
    QTcpServer *tcpServer{};
    QPushButton *shutdownButton;
    QPushButton *hideButton;
    QPushButton *runButton;
    PlayerManager *playerManager;
    QVBoxLayout *mainLayout;
    QListWidget *addressList;

    std::atomic<int> nextId{START_ID};
};
