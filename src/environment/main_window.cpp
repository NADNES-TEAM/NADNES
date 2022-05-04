#include "environment/main_window.h"
#include <QFileDialog>
#include <QMap>
#include <QMenuBar>
#include <QTimer>
#include <bitset>
#include <iostream>
#include "nes_exceptions.h"
#include "nes_properties.h"

namespace NES {

QMap<Qt::Key, int> MainWindow::m_index_by_key{{Qt::Key::Key_A, 0},
                                              {Qt::Key::Key_S, 1},
                                              {Qt::Key::Key_Tab, 2},
                                              {Qt::Key::Key_Return, 3},
                                              {Qt::Key::Key_Up, 4},
                                              {Qt::Key::Key_Down, 5},
                                              {Qt::Key::Key_Left, 6},
                                              {Qt::Key::Key_Right, 7}};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    auto pressed_key = static_cast<const Qt::Key>(event->key());
    switch (pressed_key) {
        case Qt::Key::Key_Escape:
            if (pause) {
                m_clock.start();
            } else {
                m_clock.stop();
            }
            pause = !pause;
            break;
        default: {
            unsigned value = m_index_by_key.value(pressed_key, 8);
            std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
            m_pressed_keys_bitset |= mask;              // 2 operations but still ok
        }
    }
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

MainWindow::MainWindow()
    : QMainWindow(),
      m_screen_image(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGB888),
      m_image_label(new QLabel),
      m_clock(this)
     {
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
//    std::cout << rom_path.toStdString() << '\n';
    try {
        m_clock.stop();
        m_nes = std::make_unique<Nes>(
             get_screen_interface(), get_keyboard_interface());
//        m_nes = Nes(this, this);
        m_nes->load_cartridge(rom_path.toStdString());
        m_nes->reset();
        m_clock.callOnTimeout([&]() { this->m_nes->tick(); });
        m_clock.start();
    } catch (NES::NesError &e) { std::cout << e.what(); }
}

void MainWindow::create_menus() {
    nes_menu = menuBar()->addMenu("NADNES");
    nes_menu->addAction(load_act);
}

void MainWindow::create_actions() {
    load_act = new QAction("Load ROM...", this);
    load_act->setShortcuts(QKeySequence::Open);
    load_act->setStatusTip("Select and load existing ROM file");
    connect(load_act, &QAction::triggered, this, &MainWindow::load_rom);
}

ScreenInterface *MainWindow::get_screen_interface() {
    return dynamic_cast<ScreenInterface *>(this);
}
KeyboardInterface *MainWindow::get_keyboard_interface() {
    return dynamic_cast<KeyboardInterface *>(this);
}


}  // namespace NES
