#pragma once

#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QtGui>
#include <memory>
#include <mutex>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"
#include "nes.h"

namespace NES {

enum class Keys : int { A, B, Select, Start, Up, Down, Left, Right, None };

class MainWindow : public QMainWindow, public ScreenInterface, public KeyboardInterface {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override = default;

    void closeEvent(QCloseEvent *event) override;

    // Keyboard methods
    KeyboardInterface *get_keyboard_interface();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    [[nodiscard]] uint8_t get_pressed_keys() const override;

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
    void open_keymap();

private:
    void create_menus();
    void create_actions();
    void read_settings();
    void write_settings();

    QLabel *m_image_label;
    QImage m_screen_image;

    QMenu *nes_menu;
    QAction *load_act;
    QAction *reset_act;
    QAction *pause_act;
    QTimer m_clock;
    std::unique_ptr<Nes> m_nes = nullptr;
    bool pause = false;

    QMenu *saves_menu;
    QAction *save_to_act;
    QAction *load_from_act;
    QAction *quickload_act;
    QAction *quicksave_act;
    QString last_rom_path;
    QString last_save_path;

    QMenu *settings_menu;
    QAction *open_keymap_act;
    QWidget *keymap_window;

    static QMap<Qt::Key, NES::Keys> m_index_by_key;
    std::atomic<uint8_t> m_pressed_keys_bitset{};
};

}  // namespace NES
