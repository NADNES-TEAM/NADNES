#include "environment/main_window.h"
#include <QFileDialog>
#include <QMap>
#include <QMenuBar>
#include <QMessageBox>
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

QMap<Qt::Key, int> MainWindow::m_index_by_key{{Qt::Key::Key_A, 0},
                                              {Qt::Key::Key_S, 1},
                                              {Qt::Key::Key_Tab, 2},
                                              {Qt::Key::Key_Return, 3},
                                              {Qt::Key::Key_Up, 4},
                                              {Qt::Key::Key_Down, 5},
                                              {Qt::Key::Key_Left, 6},
                                              {Qt::Key::Key_Right, 7}};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    unsigned value = m_index_by_key.value(static_cast<const Qt::Key>(event->key()), 8);
    std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
    m_pressed_keys_bitset |= mask;              // 2 operations but still ok
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    unsigned value = m_index_by_key.value(static_cast<const Qt::Key>(event->key()), 8);
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
}

void MainWindow::refresh_screen() {
    QSize qSize(SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5);
    m_image_label->setPixmap(
        QPixmap::fromImage(m_screen_image.scaled(qSize, Qt::AspectRatioMode::KeepAspectRatio)));
    m_image_label->update();
}

void MainWindow::load_rom() {
    QString rom_path = QFileDialog::getOpenFileName(this, "Select ROM", "", "iNes files (*.nes)");
    if (rom_path.isEmpty())
        return;
    try {
        m_clock.stop();
        m_nes = std::make_unique<Nes>(rom_path.toStdString(),
                                      get_screen_interface(),
                                      get_keyboard_interface());
        m_clock.callOnTimeout([&]() {
            try {
                this->m_nes->tick();
            } catch (NesError &e) { handle_exception(e); }
        });
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
    QString load_path =
        QFileDialog::getOpenFileName(this, "Select save file", "", "NADNES saves (*.save)");
    if (load_path.isEmpty()) {
        return;
    }
    last_save_path = load_path;
    m_nes->load(load_path.toStdString());
}

void MainWindow::quickload() {
    if (last_save_path.isEmpty()) {
        load_game_from();
        return;
    }
    m_nes->load(last_save_path.toStdString());
}

void MainWindow::create_menus() {
    nes_menu = menuBar()->addMenu("Emulator");
    nes_menu->addAction(load_act);
    nes_menu->addAction(reset_act);
    nes_menu->addAction(pause_act);

    saves_menu = menuBar()->addMenu("Game saves");
    saves_menu->addAction(quicksave_act);
    saves_menu->addAction(save_to_act);
    saves_menu->addSeparator();
    saves_menu->addAction(quickload_act);
    saves_menu->addAction(load_from_act);
}

void MainWindow::create_actions() {
    load_act = new QAction("Load ROM...", this);
    load_act->setShortcuts(QKeySequence::Open);
    load_act->setStatusTip("Select and load existing ROM file");
    connect(load_act, &QAction::triggered, this, &MainWindow::load_rom);

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
}

}  // namespace NES
