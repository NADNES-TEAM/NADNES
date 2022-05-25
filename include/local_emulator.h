#pragma once

#include "QObject"
#include "QTimer"
#include "environment/main_window.h"
#include "nes.h"

class LocalEmulator : public QObject {
    Q_OBJECT

public:
    LocalEmulator(NES::ScreenInterface *screen,
                  NES::KeyboardInterface *gp1,
                  NES::KeyboardInterface *gp2);
    ~LocalEmulator() override = default;

public slots:
    void load_rom(QString path = "");
    void reset_nes();
    void pause_nes();
    void save_game_to();
    void load_game_from();
    void quicksave();
    void quickload();
    void close();

private:
    void read_settings();
    void write_settings();

    QTimer m_clock;
    std::unique_ptr<NES::Nes> m_nes = nullptr;
    bool m_pause_flag = false;

    NES::ScreenInterface *m_screen;
    NES::KeyboardInterface *m_gamepad_1;
    NES::KeyboardInterface *m_gamepad_2;

    QString m_last_rom_path;
    QString m_last_save_path;
};