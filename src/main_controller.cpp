#include "main_controller.h"
#include <QAction>

MainController::MainController()
    : m_local_emulator(m_main_window.get_screen_interface(),
                       m_player1.get_keyboard_interface(),
                       m_player2.get_keyboard_interface()) {
    make_connections();
    m_player1.load_player(NES::Players::SinglePlayer);
    m_main_window.show();
}

void MainController::make_connections() {
    connect(m_main_window.m_load_act, &QAction::triggered, &m_local_emulator, [this]() {
        m_local_emulator.load_rom();
    });
    connect(m_main_window.m_reset_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::reset_nes);
    connect(m_main_window.m_pause_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::pause_nes);
    connect(m_main_window.m_save_to_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::save_game_to);
    connect(m_main_window.m_quicksave_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::quicksave);
    connect(m_main_window.m_load_from_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::load_game_from);
    connect(m_main_window.m_quickload_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::quickload);
    connect(m_main_window.m_open_pl1_keymap_act,
            &QAction::triggered,
            &m_player1,
            &Gamepad::show_editor);
    connect(m_main_window.m_open_pl2_keymap_act,
            &QAction::triggered,
            &m_player2,
            &Gamepad::show_editor);
    connect(&m_main_window, &MainWindow::key_pressed, &m_player1, &Gamepad::key_pressed);
    connect(&m_main_window, &MainWindow::key_pressed, &m_player2, &Gamepad::key_pressed);
    connect(&m_main_window, &MainWindow::key_released, &m_player1, &Gamepad::key_released);
    connect(&m_main_window, &MainWindow::key_released, &m_player2, &Gamepad::key_released);
    connect(&m_main_window, &MainWindow::closed, this, &MainController::close);
    connect(m_main_window.m_open_player_select_act,
            &QAction::triggered,
            &m_local_emulator,
            &LocalEmulator::show_player_select);
}

void MainController::close() {
    m_local_emulator.close();
    m_player1.close();
    m_player2.close();
}
