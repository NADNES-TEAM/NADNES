#pragma once

#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QtGui>
#include <memory>
#include <mutex>
#include "environment/gamepad.h"
#include "interfaces/screen_interface.h"
#include "nes.h"

namespace NES {

class MainWindow : public QMainWindow, public ScreenInterface {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override = default;

    void closeEvent(QCloseEvent *event) override;

    // Keyboard methods
    KeyboardInterface *get_pl1_keyboard_interface();
    KeyboardInterface *get_pl2_keyboard_interface();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Screen methods
    ScreenInterface *get_screen_interface();
    void set_pixel(int row, int column, Color color) override;
    void refresh_screen() override;

private slots:
    void load_rom(QString path);
    void reset_nes();
    void pause_nes();
    void save_game_to();
    void load_game_from();
    void quicksave();
    void quickload();
    void open_pl1_keymap();
    void open_pl2_keymap();
    void switch_to_single_player();
    void switch_to_cooperative();

private:
    void create_menus();
    void create_actions();
    void read_settings();
    void write_settings();

    QLabel *m_image_label;
    QImage m_screen_image;

    QMenu *m_nes_menu;
    QAction *m_load_act;
    QAction *m_reset_act;
    QAction *m_pause_act;
    QTimer m_clock;
    std::unique_ptr<Nes> m_nes = nullptr;
    bool m_pause_flag = false;

    QMenu *m_saves_menu;
    QAction *m_save_to_act;
    QAction *m_load_from_act;
    QAction *m_quickload_act;
    QAction *m_quicksave_act;
    QString m_last_rom_path;
    QString m_last_save_path;

    QMenu *m_settings_menu;
    QAction *m_open_pl1_keymap_act;
    QAction *m_open_pl2_keymap_act;
    QAction *m_single_player_act;
    QAction *m_coop_player_act;
    QActionGroup *m_profile_group;
    QMenu *m_profile_menu;
    Gamepad m_player1_gp;
    Gamepad m_player2_gp;
};

}  // namespace NES
