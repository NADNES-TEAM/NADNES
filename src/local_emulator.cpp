#include "local_emulator.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <iostream>
#include "environment/cheating/apply_cheat.h"
#include "environment/cheating/search_cheat.h"
#include "environment/player_manager_window.h"
#include "nes_properties.h"

void handle_exception(std::exception &e) {
    std::cout << e.what() << '\n';
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

LocalEmulator::LocalEmulator(QObject *parent,
                             NES::ScreenInterface *screen,
                             NES::KeyboardInterface *gp1,
                             NES::KeyboardInterface *gp2)
    : QObject(parent), m_clock(this) {
    m_clock.setInterval(
        std::chrono::milliseconds(lround(1000.0 / NES::PPU_VERTICAL_FRAME_RATE_FREQ_HZ)));

    m_player_manager = new PlayerManager();
    m_player_manager->add_screen(0, screen);
    m_player_manager->add_keyboard(0, gp1);
    m_player_manager->add_keyboard(1, gp2);
    m_player_manager->add_pseudonym(0, tr("Local"));
    m_player_manager->add_pseudonym(1, tr("Second player"));
    m_player_manager->on_cancel_btn_clicked();

    m_server = new Server(m_player_manager);

    read_settings();
    QString tmp = m_last_save_path;
    if (!m_last_rom_path.isEmpty() &&
        QMessageBox::Yes ==
            QMessageBox::question(nullptr,
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
        quick_load();
    }
}

void LocalEmulator::write_settings() {
    QSettings settings("NAD", "NADNES");
    settings.setValue("last_loaded_rom", m_last_rom_path);
    settings.setValue("last_save_file", m_last_save_path);
}

void LocalEmulator::read_settings() {
    QSettings settings("NAD", "NADNES");
    m_last_rom_path = settings.value("last_loaded_rom", "").toString();
    m_last_save_path = settings.value("last_save_file", "").toString();
}

void LocalEmulator::load_rom(QString path) {
    if (path.isEmpty()) {
        path = QFileDialog::getOpenFileName(nullptr, "Select ROM", "", "iNes files (*.nes)");
    }
    if (path.isEmpty())
        return;
    try {
        m_clock.stop();
        m_nes = std::make_unique<NES::Nes>(path.toStdString(),
                                           m_player_manager->get_screen(),
                                           m_player_manager->get_gamepad_wrapper1(),
                                           m_player_manager->get_gamepad_wrapper2());
        m_clock.callOnTimeout([&]() {
            try {
                this->m_nes->tick();
            }catch (NES::IncorrectOpcodeError &e){
                return;
            }
            catch (NES::NesError &e) {
                handle_exception(e);
            }
        });
        m_last_rom_path = path;
        m_last_save_path = "";
        m_clock.start();
    } catch (NES::NesError &e) { handle_exception(e); }
}

void LocalEmulator::pause_nes() {
    if (m_pause_flag) {
        m_clock.start();
    } else {
        m_clock.stop();
    }
    m_pause_flag = !m_pause_flag;
}

void LocalEmulator::reset_nes() {
    if (m_nes) {
        m_nes->reset();
        if (m_pause_flag) {
            m_clock.start();
            m_pause_flag = false;
        }
    }
}

void LocalEmulator::save_game_to() {
    if (!m_nes) {
        return;
    }
    PauseHolder ph(m_clock);
    QString save_path =
        QFileDialog::getSaveFileName(nullptr, "Select save file", "", "NADNES saves (*.save)");
    if (save_path.isEmpty()) {
        return;
    }
    m_last_save_path = save_path;
    m_nes->save(save_path.toStdString());
}

void LocalEmulator::quick_save() {
    if (!m_nes) {
        return;
    }
    if (m_last_save_path.isEmpty()) {
        save_game_to();
        return;
    }
    m_nes->save(m_last_save_path.toStdString());
}

void LocalEmulator::load_game_from() {
    if (!m_nes) {
        return;
    }
    PauseHolder ph(m_clock);
    QString load_path =
        QFileDialog::getOpenFileName(nullptr, "Select save file", "", "NADNES saves (*.save)");
    if (load_path.isEmpty()) {
        return;
    }
    m_last_save_path = load_path;
    m_nes->load(load_path.toStdString());
}

void LocalEmulator::quick_load() {
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

void LocalEmulator::close() {
    m_clock.stop();
    m_player_manager->close();
    m_player_manager->deleteLater();
    m_player_manager = nullptr;

    m_server->close();
    m_server->deleteLater();
    m_server = nullptr;

    if(cheat_window) {
        cheat_window->close();
        cheat_window->deleteLater();
        cheat_window = nullptr;
    }

    write_settings();
}

void LocalEmulator::show_player_select() {
    m_player_manager->show();
}

void LocalEmulator::run_server() {
    QTimer::singleShot(0, m_server, SLOT(show()));
}


void LocalEmulator::create_search_window() {
    if (m_nes == nullptr) {
        return;
    }

    if (!cheat_window) {
        cheat_window = new NES::Cheating::CheatWindow(nullptr, m_nes.get());
    }
    cheat_window->show();
}
