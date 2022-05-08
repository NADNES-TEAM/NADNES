#include "environment/main_window.h"
#include <QFileDialog>
#include <QMap>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <bitset>
#include <iostream>
#include "nes_exceptions.h"
#include "nes_properties.h"

namespace NES {

void handle_exception(std::exception &e) {
    std::cout << e.what();
    QMessageBox error_msg;
    error_msg.setText(e.what());
    error_msg.exec();
}

struct PauseHolder {
    QTimer &timer;

    explicit PauseHolder(QTimer &timer_) : timer(timer_) {
        if (timer.isActive()) {
            timer.stop();
        }
    }

    ~PauseHolder() {
        if (!timer.isActive()) {
            timer.start();
        }
    }
};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    m_player1_gp.key_pressed(Qt::Key(event->key()));
    if (m_two_players_flag) {
        m_player2_gp.key_pressed(Qt::Key(event->key()));
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    m_player1_gp.key_released(Qt::Key(event->key()));
    if (m_two_players_flag) {
        m_player2_gp.key_released(Qt::Key(event->key()));
    }
}

void MainWindow::set_pixel(int row, int column, Color color) {
    if (0 <= row && row < SCREEN_HEIGHT && 0 <= column && column < SCREEN_WIDTH) {
        m_screen_image.setPixelColor(column, row, QColor(color.r, color.g, color.b));
    }
}

ScreenInterface *MainWindow::get_screen_interface() {
    return dynamic_cast<ScreenInterface *>(this);
}

KeyboardInterface *MainWindow::get_pl1_keyboard_interface() {
    return dynamic_cast<KeyboardInterface *>(&m_player1_gp);
}

KeyboardInterface *MainWindow::get_pl2_keyboard_interface() {
    return dynamic_cast<KeyboardInterface *>(&m_player2_gp);
}

MainWindow::MainWindow()
    : QMainWindow(),
      m_screen_image(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGB888),
      m_image_label(new QLabel),
      m_clock(this) {
    m_image_label->setPixmap(QPixmap::fromImage(m_screen_image));
    m_image_label->setBackgroundRole(QPalette::Window);
    m_image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_image_label->setScaledContents(true);

    setCentralWidget(m_image_label);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    // 17 ms = (58.8235 Hz) ^ -1, ideal is 16.6393 ms = (60.0988 Hz) ^ -1
    // APU_FRAME_COUNTER_FREQ_HZ = 60
    // PPU_VERTICAL_FRAME_RATE_FREQ_HZ = 60.0988
    m_clock.setInterval(
        std::chrono::milliseconds(lround(1000.0 / NES::PPU_VERTICAL_FRAME_RATE_FREQ_HZ)));

    create_actions();
    create_menus();
    read_settings();
    m_player1_gp.load_player(Players::SinglePlayer);

    QString tmp = m_last_save_path;
    if (!m_last_rom_path.isEmpty() &&
        QMessageBox::Yes ==
            QMessageBox::question(this,
                                  "Restore session?",
                                  "Would you like to restore previous game session?",
                                  (QMessageBox::Yes | QMessageBox::No),
                                  QMessageBox::Yes)) {
        load_rom(m_last_rom_path);
    } else {
        m_last_rom_path = "";
    }
    m_last_save_path = tmp;
    if (!m_last_save_path.isEmpty()) {
        quickload();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    write_settings();
}

void MainWindow::refresh_screen() {
    QSize qSize(SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5);
    m_image_label->setPixmap(
        QPixmap::fromImage(m_screen_image.scaled(qSize, Qt::AspectRatioMode::KeepAspectRatio)));
    m_image_label->update();
}

void MainWindow::write_settings() {
    QSettings settings("NAD", "NADNES");
    settings.setValue("last_loaded_rom", m_last_rom_path);
    settings.setValue("last_save_file", m_last_save_path);
}

void MainWindow::read_settings() {
    QSettings settings("NAD", "NADNES");
    m_last_rom_path = settings.value("last_loaded_rom", "").toString();
    m_last_save_path = settings.value("last_save_file", "").toString();
}

void MainWindow::load_rom(QString path = "") {
    if (path.isEmpty()) {
        path = QFileDialog::getOpenFileName(this, "Select ROM", "", "iNes files (*.nes)");
    }
    if (path.isEmpty())
        return;
    try {
        m_clock.stop();
        m_nes = std::make_unique<Nes>(path.toStdString(),
                                      get_screen_interface(),
                                      get_pl1_keyboard_interface(),
                                      get_pl2_keyboard_interface());
        m_clock.callOnTimeout([&]() {
            try {
                this->m_nes->tick();
            } catch (NesError &e) { handle_exception(e); }
        });
        m_last_rom_path = path;
        m_last_save_path = "";
        m_clock.start();
    } catch (NES::NesError &e) { handle_exception(e); }
}

void MainWindow::pause_nes() {
    if (m_pause_flag) {
        m_clock.start();
    } else {
        m_clock.stop();
    }
    m_pause_flag = !m_pause_flag;
}

void MainWindow::reset_nes() {
    if (m_nes) {
        m_nes->reset();
        if (m_pause_flag) {
            m_clock.start();
            m_pause_flag = false;
        }
    }
}

void MainWindow::save_game_to() {
    if (!m_nes) {
        return;
    }
    PauseHolder ph(m_clock);
    QString save_path =
        QFileDialog::getSaveFileName(this, "Select save file", "", "NADNES saves (*.save)");
    if (save_path.isEmpty()) {
        return;
    }
    m_last_save_path = save_path;
    m_nes->save(save_path.toStdString());
}

void MainWindow::quicksave() {
    if (!m_nes) {
        return;
    }
    if (m_last_save_path.isEmpty()) {
        save_game_to();
        return;
    }
    m_nes->save(m_last_save_path.toStdString());
}

void MainWindow::load_game_from() {
    if (!m_nes) {
        return;
    }
    PauseHolder ph(m_clock);
    QString load_path =
        QFileDialog::getOpenFileName(this, "Select save file", "", "NADNES saves (*.save)");
    if (load_path.isEmpty()) {
        return;
    }
    m_last_save_path = load_path;
    m_nes->load(load_path.toStdString());
}

void MainWindow::quickload() {
    if (!m_nes) {
        return;
    }
    PauseHolder ph(m_clock);
    if (m_last_save_path.isEmpty()) {
        load_game_from();
        return;
    }
    m_nes->load(m_last_save_path.toStdString());
}

void MainWindow::open_pl1_keymap() {
    m_player1_gp.show_editor();
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

    m_settings_menu = menuBar()->addMenu("Settings");
    m_settings_menu->addAction(m_open_pl1_keymap_act);
}

void MainWindow::create_actions() {
    m_load_act = new QAction("Load ROM...", this);
    m_load_act->setShortcuts(QKeySequence::Open);
    m_load_act->setStatusTip("Select and load existing ROM file");
    connect(m_load_act, &QAction::triggered, this, [this]() { load_rom(); });

    m_reset_act = new QAction("Reset", this);
    m_reset_act->setShortcut(QKeySequence("Ctrl+R"));
    m_reset_act->setStatusTip("Reset NES state");
    connect(m_reset_act, &QAction::triggered, this, &MainWindow::reset_nes);

    m_pause_act = new QAction("Pause/Resume", this);
    m_pause_act->setShortcut(QKeySequence("Esc"));
    m_pause_act->setStatusTip("Pause or resume emulation");
    connect(m_pause_act, &QAction::triggered, this, &MainWindow::pause_nes);

    m_save_to_act = new QAction("Save game as...", this);
    m_save_to_act->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_save_to_act->setStatusTip("Save game to selected file");
    connect(m_save_to_act, &QAction::triggered, this, &MainWindow::save_game_to);

    m_quicksave_act = new QAction("Quicksave", this);
    m_quicksave_act->setShortcut(QKeySequence("Ctrl+S"));
    m_quicksave_act->setStatusTip("Save game to last selected file");
    connect(m_quicksave_act, &QAction::triggered, this, &MainWindow::quicksave);

    m_load_from_act = new QAction("Load game from...", this);
    m_load_from_act->setShortcut(QKeySequence("Ctrl+Shift+L"));
    m_load_from_act->setStatusTip("Load game from selected save file");
    connect(m_load_from_act, &QAction::triggered, this, &MainWindow::load_game_from);

    m_quickload_act = new QAction("Quickload", this);
    m_quickload_act->setShortcut(QKeySequence("Ctrl+L"));
    m_quickload_act->setStatusTip("Load game from last selected save file");
    connect(m_quickload_act, &QAction::triggered, this, &MainWindow::quickload);

    m_open_pl1_keymap_act = new QAction("Keymap", this);
    m_open_pl1_keymap_act->setShortcut(QKeySequence("Ctrl+,"));
    m_open_pl1_keymap_act->setStatusTip("Open keymap settings");
    connect(m_open_pl1_keymap_act, &QAction::triggered, this, &MainWindow::open_pl1_keymap);
}

}  // namespace NES
