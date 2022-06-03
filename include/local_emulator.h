#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QWidget>
#include "Server.h"
#include "cheat_db_handler2.h"
#include "environment/cheating/cheat_window.h"
#include "environment/player_select_window.h"
#include "nes.h"

class LocalEmulator : public QObject {
    Q_OBJECT

public:
    LocalEmulator(QObject *parent,
                  NES::ScreenInterface *screen,
                  NES::KeyboardInterface *gp1,
                  NES::KeyboardInterface *gp2);
    ~LocalEmulator() override = default;

    void close();

public slots:
    void load_rom(QString path = "");
    void reset_nes();
    void pause_nes();
    void save_game_to();
    void load_game_from();
    void quicksave();
    void quickload();
    void show_player_select();
    void run_server();
    void create_search_window();

private:
    void read_settings();
    void write_settings();

    QTimer m_clock;
    std::unique_ptr<NES::Nes> m_nes = nullptr;
    bool m_pause_flag = false;

    PlayerManager *m_player_manager;

    QString m_last_rom_path;
    QString m_last_save_path;

    Server *m_server;
    NES::Cheating::CheatWindow *cheat_window{};
};
