#include "environment/main_window.h"
#include <QFileDialog>
#include <QMap>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <bitset>
#include <iostream>
#include "environment/keymap_window.h"
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
    auto value = static_cast<unsigned>(
        m_index_by_key.value(static_cast<const Qt::Key>(event->key()), Keys::None));
    std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
    m_pressed_keys_bitset |= mask;              // 2 operations but still ok
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    auto value = static_cast<unsigned>(
        m_index_by_key.value(static_cast<const Qt::Key>(event->key()), Keys::None));
    std::atomic<uint8_t> mask = ~(1u << value);  // ~(1 << 8) == 0xFF
    m_pressed_keys_bitset &= mask;               // 2 operations but still ok
}

uint8_t MainWindow::get_pressed_keys() const {
    return m_pressed_keys_bitset;
}

void MainWindow::set_pixel(int row, int column, Color color) {
    if (0 <= row && row < SCREEN_HEIGHT && 0 <= column && column < SCREEN_WIDTH) {
        m_screen_image.setPixelColor(column, row, QColor(color.r, color.g, color.b));
    }
}

ScreenInterface *MainWindow::get_screen_interface() {
    return dynamic_cast<ScreenInterface *>(this);
}

KeyboardInterface *MainWindow::get_keyboard_interface() {
    return dynamic_cast<KeyboardInterface *>(this);
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
    QString tmp = last_save_path;
    if (!last_rom_path.isEmpty() &&
        QMessageBox::Yes ==
            QMessageBox::question(this,
                                  "Restore session?",
                                  "Would you like to restore previous game session?",
                                  (QMessageBox::Yes | QMessageBox::No),
                                  QMessageBox::Yes)) {
        load_rom(last_rom_path);
    } else {
        last_rom_path = "";
    }
    last_save_path = tmp;
    if (!last_save_path.isEmpty()) {
        quickload();
    }
    keymap_window = new KeymapWindow(m_index_by_key);
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

QMap<Qt::Key, NES::Keys> MainWindow::m_index_by_key;

void MainWindow::write_settings() {
    QSettings settings("NAD", "NADNES");
    settings.setValue("last_loaded_rom", last_rom_path);
    settings.setValue("last_save_file", last_save_path);
}

void MainWindow::read_settings() {
    QSettings settings("NAD", "NADNES");
    settings.beginGroup("keymap");
    m_index_by_key.insert(settings.value("key_0", Qt::Key::Key_A).value<Qt::Key>(), NES::Keys::A);
    m_index_by_key.insert(settings.value("key_1", Qt::Key::Key_S).value<Qt::Key>(), NES::Keys::B);
    m_index_by_key.insert(settings.value("key_2", Qt::Key::Key_Tab).value<Qt::Key>(),
                          NES::Keys::Select);
    m_index_by_key.insert(settings.value("key_3", Qt::Key::Key_Return).value<Qt::Key>(),
                          NES::Keys::Start);
    m_index_by_key.insert(settings.value("key_4", Qt::Key::Key_Up).value<Qt::Key>(), NES::Keys::Up);
    m_index_by_key.insert(settings.value("key_5", Qt::Key::Key_Down).value<Qt::Key>(),
                          NES::Keys::Down);
    m_index_by_key.insert(settings.value("key_6", Qt::Key::Key_Left).value<Qt::Key>(),
                          NES::Keys::Left);
    m_index_by_key.insert(settings.value("key_7", Qt::Key::Key_Right).value<Qt::Key>(),
                          NES::Keys::Right);
    settings.endGroup();
    last_rom_path = settings.value("last_loaded_rom", "").toString();
    last_save_path = settings.value("last_save_file", "").toString();
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
                                      get_keyboard_interface());
        m_clock.callOnTimeout([&]() {
            try {
                this->m_nes->tick();
            } catch (NesError &e) { handle_exception(e); }
        });
        last_rom_path = path;
        last_save_path = "";
        m_clock.start();
    } catch (NES::NesError &e) { handle_exception(e); }
}

void MainWindow::pause_nes() {
    if (pause) {
        m_clock.start();
    } else {
        m_clock.stop();
    }
    pause = !pause;
}

void MainWindow::reset_nes() {
    if (m_nes) {
        m_nes->reset();
        if (pause) {
            m_clock.start();
            pause = false;
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
    last_save_path = save_path;
    m_nes->save(save_path.toStdString());
}

void MainWindow::quicksave() {
    if (!m_nes) {
        return;
    }
    if (last_save_path.isEmpty()) {
        save_game_to();
        return;
    }
    m_nes->save(last_save_path.toStdString());
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
    last_save_path = load_path;
    m_nes->load(load_path.toStdString());
}

void MainWindow::quickload() {
    if (!m_nes) {
        return;
    }
    PauseHolder ph(m_clock);
    if (last_save_path.isEmpty()) {
        load_game_from();
        return;
    }
    m_nes->load(last_save_path.toStdString());
}

void MainWindow::open_keymap() {
    keymap_window->show();
}

void MainWindow::create_menus() {
    nes_menu = menuBar()->addMenu("Emulator");
    nes_menu->addAction(load_act);
    nes_menu->addAction(reset_act);
    nes_menu->addAction(pause_act);

    saves_menu = menuBar()->addMenu("Saves");
    saves_menu->addAction(quicksave_act);
    saves_menu->addAction(save_to_act);
    saves_menu->addSeparator();
    saves_menu->addAction(quickload_act);
    saves_menu->addAction(load_from_act);

    settings_menu = menuBar()->addMenu("Settings");
    settings_menu->addAction(open_keymap_act);
}

void MainWindow::create_actions() {
    load_act = new QAction("Load ROM...", this);
    load_act->setShortcuts(QKeySequence::Open);
    load_act->setStatusTip("Select and load existing ROM file");
    connect(load_act, &QAction::triggered, this, [this]() { load_rom(); });

    reset_act = new QAction("Reset", this);
    reset_act->setShortcut(QKeySequence("Ctrl+R"));
    reset_act->setStatusTip("Reset NES state");
    connect(reset_act, &QAction::triggered, this, &MainWindow::reset_nes);

    pause_act = new QAction("Pause/Resume", this);
    pause_act->setShortcut(QKeySequence("Esc"));
    pause_act->setStatusTip("Pause or resume emulation");
    connect(pause_act, &QAction::triggered, this, &MainWindow::pause_nes);

    save_to_act = new QAction("Save game as...", this);
    save_to_act->setShortcut(QKeySequence("Ctrl+Shift+S"));
    save_to_act->setStatusTip("Save game to selected file");
    connect(save_to_act, &QAction::triggered, this, &MainWindow::save_game_to);

    quicksave_act = new QAction("Quicksave", this);
    quicksave_act->setShortcut(QKeySequence("Ctrl+S"));
    quicksave_act->setStatusTip("Save game to last selected file");
    connect(quicksave_act, &QAction::triggered, this, &MainWindow::quicksave);

    load_from_act = new QAction("Load game from...", this);
    load_from_act->setShortcut(QKeySequence("Ctrl+Shift+L"));
    load_from_act->setStatusTip("Load game from selected save file");
    connect(load_from_act, &QAction::triggered, this, &MainWindow::load_game_from);

    quickload_act = new QAction("Quickload", this);
    quickload_act->setShortcut(QKeySequence("Ctrl+L"));
    quickload_act->setStatusTip("Load game from last selected save file");
    connect(quickload_act, &QAction::triggered, this, &MainWindow::quickload);

    open_keymap_act = new QAction("Keymap", this);
    open_keymap_act->setShortcut(QKeySequence("Ctrl+,"));
    open_keymap_act->setStatusTip("Open keymap settings");
    connect(open_keymap_act, &QAction::triggered, this, &MainWindow::open_keymap);
}

}  // namespace NES
