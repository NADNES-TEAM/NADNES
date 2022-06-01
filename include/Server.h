#pragma once

#include <QDialog>
#include <QLabel>
#include <QTcpServer>
#include <QListWidget>
#include "environment/player_select_window.h"
#include "remote_player.h"
#include <QVBoxLayout>

class Server : public QDialog {
public:
    explicit Server(PlayerManager *player_manager, QWidget *parent = nullptr);

    void open_connections() const;

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
    PlayerManager *m_player_manager;
    QVBoxLayout *main_layout;
    QListWidget *address_list;

    std::atomic<int> m_next_id{START_ID};
};
