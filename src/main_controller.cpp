#include "main_controller.h"
#include <QAction>

MainController::MainController() {
    make_common_connections();
    become_host();
    m_player1.load_player(NES::Players::SinglePlayer);
    m_main_window.show();
}

void MainController::make_common_connections() {
    connect(m_main_window.m_become_host_act, SIGNAL(triggered()), this, SLOT(become_host()));
    connect(m_main_window.m_become_guest_act, SIGNAL(triggered()), this, SLOT(become_guest()));
    connect(&m_main_window, &MainWindow::key_pressed, &m_player1, &Gamepad::key_pressed);
    connect(&m_main_window, &MainWindow::key_pressed, &m_player2, &Gamepad::key_pressed);
    connect(&m_main_window, &MainWindow::key_released, &m_player1, &Gamepad::key_released);
    connect(&m_main_window, &MainWindow::key_released, &m_player2, &Gamepad::key_released);
    connect(&m_main_window, &MainWindow::closed, this, &MainController::close);
    connect(m_main_window.m_open_pl1_keymap_act,
            &QAction::triggered,
            &m_player1,
            &Gamepad::show_editor);
    connect(m_main_window.m_open_pl2_keymap_act,
            &QAction::triggered,
            &m_player2,
            &Gamepad::show_editor);
    connect(m_main_window.m_player1_single_act, &QAction::triggered, this, [this]() {
        m_player1.load_player(NES::Players::SinglePlayer);
        m_player2.load_player(NES::Players::None);
    });
    connect(m_main_window.m_player2_single_act, &QAction::triggered, this, [this]() {
        m_player1.load_player(NES::Players::None);
        m_player2.load_player(NES::Players::SinglePlayer);
    });
    connect(m_main_window.m_coop_player_act, &QAction::triggered, this, [this]() {
        m_player1.load_player(NES::Players::Player1);
        m_player2.load_player(NES::Players::Player2);
    });
}

void MainController::make_host_connections() {
    connect(m_main_window.m_load_act, &QAction::triggered, m_local_emulator, [this]() {
        m_local_emulator->load_rom();
    });
    connect(m_main_window.m_reset_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::reset_nes);
    connect(m_main_window.m_pause_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::pause_nes);
    connect(m_main_window.m_save_to_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::save_game_to);
    connect(m_main_window.m_quicksave_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::quicksave);
    connect(m_main_window.m_load_from_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::load_game_from);
    connect(m_main_window.m_quickload_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::quickload);
    connect(m_main_window.m_open_player_select_act,
            &QAction::triggered,
            m_local_emulator,
            &LocalEmulator::show_player_select);
    connect(m_main_window.m_run_server_act,
            SIGNAL(triggered()),
            m_local_emulator,
            SLOT(run_server()));
}

void MainController::make_guest_connections() {
    connect(m_main_window.m_connect_to_host_act,
            &QAction::triggered,
            m_remote_emulator,
            &RemoteEmulator::show_connection_window);
    connect(&m_player1, &Gamepad::state_changed, m_remote_emulator, &RemoteEmulator::key_changed);
}

void MainController::close() {
    m_local_emulator->close();
    m_player1.close();
    m_player2.close();
}

void MainController::become_host() {
    m_main_window.enable_actions(ActionRole::Host);
    m_remote_emulator->deleteLater();
    m_local_emulator = new LocalEmulator(this,
                                         m_main_window.get_screen_interface(),
                                         m_player1.get_keyboard_interface(),
                                         m_player2.get_keyboard_interface());
    make_host_connections();
}

void MainController::become_guest() {
    m_main_window.enable_actions(ActionRole::Guest);
    m_local_emulator->deleteLater();
    m_remote_emulator = new RemoteEmulator(this, m_main_window.get_screen_interface());
    make_guest_connections();
}
