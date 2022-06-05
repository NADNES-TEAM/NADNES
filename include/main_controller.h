#pragma once

#include <QObject>
#include "environment/main_window.h"
#include "gamepad.h"
#include "local_emulator.h"
#include "remote_emulator.h"

struct MainController : public QObject {
    Q_OBJECT

public:
    MainController();
    ~MainController() override = default;

public slots:
    void close();
    void become_host();
    void become_guest();

private:
    void make_host_connections();
    void make_common_connections();
    void make_guest_connections();
    MainWindow m_main_window;
    Gamepad m_player1;
    Gamepad m_player2;
    LocalEmulator *m_local_emulator{};
    RemoteEmulator *m_remote_emulator{};
};
