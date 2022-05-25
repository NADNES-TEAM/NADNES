#pragma once

#include "QObject"
#include "environment/main_window.h"
#include "gamepad.h"
#include "local_emulator.h"

struct MainController : public QObject {
    Q_OBJECT

public:
    MainController();
    ~MainController() override = default;

private slots:

private:
    void make_connections();

    MainWindow m_main_window;
    Gamepad m_player1;
    Gamepad m_player2;
    LocalEmulator m_local_emulator;
};