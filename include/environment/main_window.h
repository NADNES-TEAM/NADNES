#pragma once

#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QtGui>
#include "gamepad.h"
#include "interfaces/screen_interface.h"

class MainWindow : public QMainWindow, public NES::ScreenInterface {
    Q_OBJECT

signals:
    void key_pressed(Qt::Key key);
    void key_released(Qt::Key key);
    void closed();

public:
    MainWindow();
    ~MainWindow() override = default;

    void closeEvent(QCloseEvent *event) override;

    // Keyboard methods
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Screen methods
    ScreenInterface *get_screen_interface();
    void set_pixel(int row, int column, NES::Color color) override;
    void refresh_screen() override;

    void disable_all_actions();
    void enable_local_actions();
    void enable_remote_actions();

    QAction *m_load_act;
    QAction *m_reset_act;
    QAction *m_pause_act;

    QAction *m_save_to_act;
    QAction *m_load_from_act;
    QAction *m_quickload_act;
    QAction *m_quicksave_act;

    QAction *m_open_pl1_keymap_act;
    QAction *m_open_pl2_keymap_act;
    QAction *m_open_player_select_act;

    QAction *m_become_host;
    QAction *m_become_guest;

private:
    void create_menus();
    void create_actions();

    QLabel *m_image_label;
    QImage m_screen_image;

    QMenu *m_nes_menu;

    QMenu *m_saves_menu;

    QMenu *m_settings_menu;

    QMenu *m_host_guest_menu;
};
