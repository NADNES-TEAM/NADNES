#pragma once

#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <mutex>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"

class PlayerManager : public QWidget, public NES::ScreenInterface {
    Q_OBJECT

public:
    PlayerManager();
    ~PlayerManager() override = default;

    void add_player(int index, const QString &name);
    void add_screen(int id, NES::ScreenInterface *screen);
    void add_keyboard(int id, NES::KeyboardInterface *gamepad);
    void update_gamepads();

    void make_connections();

    void set_pixel(int row, int column, NES::Color color) override;
    void refresh_screen() override;

    NES::KeyboardInterface *get_gamepad_wrapper1();
    NES::KeyboardInterface *get_gamepad_wrapper2();
    NES::ScreenInterface *get_screen();

public slots:
    void remove_player(int index);
    void on_cancel_btn_clicked();
    void update_view_and_indexes();
private slots:
    void on_ok_btn_clicked();
    void show_settings();

private:
//    std::mutex m_mutex;
    int m_player1_id = 0;  // whatever
    int m_player2_id = 1;  // whatever
    QComboBox *m_player1_select;
    QComboBox *m_player2_select;
    std::vector<int> m_view_ids;
    QPushButton *m_ok_btn;
    QPushButton *m_cancel_btn;

    std::map<int, NES::ScreenInterface *> m_screens_map;
    std::map<int, NES::KeyboardInterface *> m_gamepads_map;

    QMap<int, QString> m_name_by_id;

    struct GamepadWrapper : NES::KeyboardInterface {
        [[nodiscard]] uint8_t get_pressed_keys() const override;
        void set_gamepad(NES::KeyboardInterface *new_gamepad);

    private:
        NES::KeyboardInterface *m_gamepad{};
    } m_gamepad_wrapper1, m_gamepad_wrapper2;
};