#include "environment/main_window.h"
#include <QMap>
#include <QMenuBar>
#include <bitset>
#include "nes_properties.h"

void MainWindow::keyPressEvent(QKeyEvent *event) {
    emit key_pressed(Qt::Key(event->key()));
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    emit key_released(Qt::Key(event->key()));
}

void MainWindow::set_pixel(int row, int column, NES::Color color) {
    if (0 <= row && row < NES::SCREEN_HEIGHT && 0 <= column && column < NES::SCREEN_WIDTH) {
        m_screen_image.setPixelColor(column, row, QColor(color.r, color.g, color.b));
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

    // 17 ms = (58.8235 Hz) ^ -1, ideal is 16.6393 ms = (60.0988 Hz) ^ -1
    // APU_FRAME_COUNTER_FREQ_HZ = 60
    // PPU_VERTICAL_FRAME_RATE_FREQ_HZ = 60.0988

    create_actions();
    create_menus();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit closed();
}

void MainWindow::refresh_screen() {
    QSize qSize(NES::SCREEN_WIDTH * 5, NES::SCREEN_HEIGHT * 5);
    m_image_label->setPixmap(
        QPixmap::fromImage(m_screen_image.scaled(qSize, Qt::AspectRatioMode::KeepAspectRatio)));
    m_image_label->update();
}

void MainWindow::create_menus() {
    m_nes_menu = menuBar()->addMenu("Emulator");
    m_nes_menu->addAction(m_load_act);
    m_nes_menu->addAction(m_reset_act);
    m_nes_menu->addAction(m_pause_act);

    m_saves_menu = menuBar()->addMenu("Saves");
    m_saves_menu->addAction(m_quicksave_act);
    m_saves_menu->addAction(m_save_to_act);
    m_saves_menu->addSeparator();
    m_saves_menu->addAction(m_quickload_act);
    m_saves_menu->addAction(m_load_from_act);

    m_settings_menu = menuBar()->addMenu("Controls");
    m_settings_menu->addAction(m_open_pl1_keymap_act);
    m_settings_menu->addAction(m_open_pl2_keymap_act);
    m_settings_menu->addAction(m_open_player_select_act);

    m_host_guest_menu = menuBar()->addMenu("Mode");
    m_host_guest_menu->addAction(m_become_host_act);
    m_host_guest_menu->addAction(m_become_guest_act);

    m_server_menu = menuBar()->addMenu(tr("Server"));
    m_server_menu->addAction(m_run_server_act);
}

void MainWindow::create_actions() {
    m_load_act = new QAction("Load ROM...", this);
    m_load_act->setShortcuts(QKeySequence::Open);
    m_load_act->setStatusTip("Select and load existing ROM file");

    m_reset_act = new QAction("Reset", this);
    m_reset_act->setShortcut(QKeySequence("Ctrl+R"));
    m_reset_act->setStatusTip("Reset NES state");

    m_pause_act = new QAction("Pause/Resume", this);
    m_pause_act->setShortcut(QKeySequence("Esc"));
    m_pause_act->setStatusTip("Pause or resume emulation");

    m_save_to_act = new QAction("Save game as...", this);
    m_save_to_act->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_save_to_act->setStatusTip("Save game to selected file");

    m_quicksave_act = new QAction("Quicksave", this);
    m_quicksave_act->setShortcut(QKeySequence("Ctrl+S"));
    m_quicksave_act->setStatusTip("Save game to last selected file");

    m_load_from_act = new QAction("Load game from...", this);
    m_load_from_act->setShortcut(QKeySequence("Ctrl+Shift+L"));
    m_load_from_act->setStatusTip("Load game from selected save file");

    m_quickload_act = new QAction("Quickload", this);
    m_quickload_act->setShortcut(QKeySequence("Ctrl+L"));
    m_quickload_act->setStatusTip("Load game from last selected save file");

    m_open_pl1_keymap_act = new QAction("Player 1", this);
    m_open_pl1_keymap_act->setShortcut(QKeySequence("Ctrl+1"));
    m_open_pl1_keymap_act->setStatusTip("Open player 1 keymap settings");

    m_open_pl2_keymap_act = new QAction("Player 2", this);
    m_open_pl2_keymap_act->setShortcut(QKeySequence("Ctrl+2"));
    m_open_pl2_keymap_act->setStatusTip("Open player 2 keymap settings");

    m_open_player_select_act = new QAction("Player select...", this);
    m_open_player_select_act->setShortcut(QKeySequence("Ctrl+P"));
    m_open_player_select_act->setStatusTip("Open player select menu");

    m_become_host_act = new QAction(tr("Become host"), this);
    m_become_host_act->setStatusTip(tr("Host is the person who runs NES"));

    m_become_guest_act = new QAction(tr("Become guest"), this);
    m_become_guest_act->setStatusTip(
        tr("Guest is the person who doesn't run NES and just sends/receives data"));

    m_run_server_act = new QAction(tr("Run server"));
    m_run_server_act->setStatusTip(tr("Runs server"));
}

void MainWindow::disable_all_actions() {
    m_load_act->setEnabled(false);
    m_reset_act->setEnabled(false);
    m_pause_act->setEnabled(false);
    m_save_to_act->setEnabled(false);
    m_quickload_act->setEnabled(false);
    m_load_from_act->setEnabled(false);
    m_quicksave_act->setEnabled(false);
    m_open_pl1_keymap_act->setEnabled(false);
    m_open_pl2_keymap_act->setEnabled(false);
    m_open_player_select_act->setEnabled(false);
    m_become_host_act->setEnabled(false);
    m_become_guest_act->setEnabled(false);
    m_run_server_act->setEnabled(false);
}

void MainWindow::enable_local_actions() {
    m_load_act->setEnabled(true);
    m_reset_act->setEnabled(true);
    m_pause_act->setEnabled(true);
    m_save_to_act->setEnabled(true);
    m_quickload_act->setEnabled(true);
    m_load_from_act->setEnabled(true);
    m_quicksave_act->setEnabled(true);
    m_open_pl1_keymap_act->setEnabled(true);
    m_open_pl2_keymap_act->setEnabled(true);
    m_open_player_select_act->setEnabled(true);
    m_become_guest_act->setEnabled(true);
    m_run_server_act->setEnabled(true);
}

void MainWindow::enable_remote_actions() {
    m_become_host_act->setEnabled(true);
    m_open_pl1_keymap_act->setEnabled(true);
}
