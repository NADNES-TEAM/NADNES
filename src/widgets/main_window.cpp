#include "widgets/main_window.h"
#include <QMenuBar>

void MainWindow::keyPressEvent(QKeyEvent *event) {
    emit key_pressed(Qt::Key(event->key()));
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    emit key_released(Qt::Key(event->key()));
}

void MainWindow::set_pixel(int row, int column, NES::Color color) {
    if (0 < row && row < NES::SCREEN_HEIGHT && 0 < column && column <= NES::SCREEN_WIDTH) {
        m_screen_image.setPixelColor(column - 1, row - 1, QColor(color.r, color.g, color.b));
    }
}

NES::ScreenInterface *MainWindow::get_screen_interface() {
    return dynamic_cast<ScreenInterface *>(this);
}

MainWindow::MainWindow()
    : QMainWindow(),
      m_screen_image(NES::SCREEN_WIDTH, NES::SCREEN_HEIGHT, QImage::Format_RGB888),
      m_image_label(new QLabel) {
    m_image_label->setPixmap(QPixmap::fromImage(m_screen_image));
    m_image_label->setBackgroundRole(QPalette::Window);
    m_image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_image_label->setScaledContents(true);

    setCentralWidget(m_image_label);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    create_actions();
    create_menus();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit closed();
}

void MainWindow::refresh_screen() {
    if (frame_count % frame_rate != 0) {
        return;
    }
    QSize qSize(NES::SCREEN_WIDTH * 5, NES::SCREEN_HEIGHT * 5);
    m_image_label->setPixmap(
        QPixmap::fromImage(m_screen_image.scaled(qSize, Qt::AspectRatioMode::KeepAspectRatio)));
}

void MainWindow::create_menus() {
    m_nes_menu = menuBar()->addMenu("Emulator");
    m_nes_menu->addAction(m_load_act);
    m_nes_menu->addAction(m_reset_act);
    m_nes_menu->addAction(m_pause_act);
    m_nes_menu->addAction(m_mem_search_act);

    m_saves_menu = menuBar()->addMenu("Saves");
    m_saves_menu->addAction(m_quick_save_act);
    m_saves_menu->addAction(m_save_to_act);
    m_saves_menu->addSeparator();
    m_saves_menu->addAction(m_quick_load_act);
    m_saves_menu->addAction(m_load_from_act);

    m_settings_menu = menuBar()->addMenu("Controls");
    m_settings_menu->addAction(m_open_pl1_keymap_act);
    m_settings_menu->addAction(m_open_pl2_keymap_act);
    m_profile_menu = m_settings_menu->addMenu("Profile");
    m_profile_menu->addAction(m_player1_single_act);
    m_profile_menu->addAction(m_player2_single_act);
    m_profile_menu->addAction(m_coop_player_act);
    m_settings_menu->addSeparator();
    m_settings_menu->addAction(m_open_player_select_act);

    m_network_menu = menuBar()->addMenu("Network");
    m_network_menu->addAction(m_become_host_act);
    m_network_menu->addAction(m_run_server_act);
    m_network_menu->addSeparator();
    m_network_menu->addAction(m_become_guest_act);
    m_network_menu->addAction(m_connect_to_host_act);
}

void MainWindow::create_actions() {
    m_load_act = new QAction("Load ROM...", this);
    m_load_act->setShortcuts(QKeySequence::Open);
    m_load_act->setStatusTip("Select and load existing ROM file");
    m_all_actions.emplace_back(m_load_act, ActionRole::Host);

    m_reset_act = new QAction("Reset", this);
    m_reset_act->setShortcut(QKeySequence("Ctrl+R"));
    m_reset_act->setStatusTip("Reset NES state");
    m_all_actions.emplace_back(m_reset_act, ActionRole::Host);

    m_pause_act = new QAction("Pause/Resume", this);
    m_pause_act->setShortcut(QKeySequence("Esc"));
    m_pause_act->setStatusTip("Pause or resume emulation");
    m_all_actions.emplace_back(m_pause_act, ActionRole::Host);

    m_save_to_act = new QAction("Save game as...", this);
    m_save_to_act->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_save_to_act->setStatusTip("Save game to selected file");
    m_all_actions.emplace_back(m_save_to_act, ActionRole::Host);

    m_quick_save_act = new QAction("Quicksave", this);
    m_quick_save_act->setShortcut(QKeySequence("Ctrl+S"));
    m_quick_save_act->setStatusTip("Save game to last selected file");
    m_all_actions.emplace_back(m_quick_save_act, ActionRole::Host);

    m_load_from_act = new QAction("Load game from...", this);
    m_load_from_act->setShortcut(QKeySequence("Ctrl+Shift+L"));
    m_load_from_act->setStatusTip("Load game from selected save file");
    m_all_actions.emplace_back(m_load_from_act, ActionRole::Host);

    m_quick_load_act = new QAction("Quickload", this);
    m_quick_load_act->setShortcut(QKeySequence("Ctrl+L"));
    m_quick_load_act->setStatusTip("Load game from last selected save file");
    m_all_actions.emplace_back(m_quick_load_act, ActionRole::Host);

    m_open_pl1_keymap_act = new QAction("Player 1...", this);
    m_open_pl1_keymap_act->setShortcut(QKeySequence("Ctrl+1"));
    m_open_pl1_keymap_act->setStatusTip("Open player 1 keymap settings");
    m_all_actions.emplace_back(m_open_pl1_keymap_act, ActionRole::Host | ActionRole::Guest);

    m_open_pl2_keymap_act = new QAction("Player 2...", this);
    m_open_pl2_keymap_act->setShortcut(QKeySequence("Ctrl+2"));
    m_open_pl2_keymap_act->setStatusTip("Open player 2 keymap settings");
    m_all_actions.emplace_back(m_open_pl2_keymap_act, ActionRole::Host);

    m_open_player_select_act = new QAction("Player select...", this);
    m_open_player_select_act->setShortcut(QKeySequence("Ctrl+P"));
    m_open_player_select_act->setStatusTip("Open player select menu");
    m_all_actions.emplace_back(m_open_player_select_act, ActionRole::Host);

    m_become_host_act = new QAction(tr("Become host"), this);
    m_become_host_act->setStatusTip(tr("Host is the person who runs NES"));
    m_all_actions.emplace_back(m_become_host_act, ActionRole::Guest);

    m_become_guest_act = new QAction(tr("Become guest"), this);
    m_become_guest_act->setStatusTip(
        tr("Guest is the person who doesn't run NES and just sends/receives data"));
    m_all_actions.emplace_back(m_become_guest_act, ActionRole::Host);

    m_run_server_act = new QAction(tr("Server..."));
    m_run_server_act->setStatusTip(tr("Open server control panel"));
    m_all_actions.emplace_back(m_run_server_act, ActionRole::Host);

    m_connect_to_host_act = new QAction(tr("Join game..."));
    m_connect_to_host_act->setStatusTip(tr("Connect to host"));
    m_all_actions.emplace_back(m_connect_to_host_act, ActionRole::Guest);

    m_player1_single_act = new QAction("Single-player 1", this);
    m_player1_single_act->setShortcut(QKeySequence("Ctrl+Shift+1"));
    m_player1_single_act->setStatusTip("Switch player 1 to single player keymap");
    m_player1_single_act->setCheckable(true);
    m_all_actions.emplace_back(m_player1_single_act, ActionRole::Host);

    m_player2_single_act = new QAction("Single-player 2", this);
    m_player2_single_act->setShortcut(QKeySequence("Ctrl+Shift+2"));
    m_player2_single_act->setStatusTip("Switch player 2 to single player keymap");
    m_player2_single_act->setCheckable(true);
    m_all_actions.emplace_back(m_player2_single_act, ActionRole::Host);

    m_coop_player_act = new QAction("Cooperative", this);
    m_coop_player_act->setShortcut(QKeySequence("Ctrl+Shift+C"));
    m_coop_player_act->setStatusTip("Switch to local cooperative keymap");
    m_coop_player_act->setCheckable(true);
    m_all_actions.emplace_back(m_coop_player_act, ActionRole::Host);

    m_profile_group = new QActionGroup(this);
    m_profile_group->addAction(m_player1_single_act);
    m_profile_group->addAction(m_player2_single_act);
    m_profile_group->addAction(m_coop_player_act);
    m_player1_single_act->setChecked(true);

    m_mem_search_act = new QAction("Cheat", this);
    m_mem_search_act->setStatusTip("Open a window with cheat options");
    m_mem_search_act->setShortcut(QKeySequence("Ctrl+Shift+M"));
    m_all_actions.emplace_back(m_mem_search_act, ActionRole::Host);
}

void MainWindow::enable_actions(ActionRole role) const {
    for (auto &[act, act_role] : m_all_actions) {
        act->setEnabled(role & act_role);
    }
}

void MainWindow::clear() {
    m_screen_image.fill(QColor("black"));
    refresh_screen();
}
