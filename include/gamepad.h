#pragma once
#include <QMap>
#include <QObject>
#include <QWidget>
#include <Qt>
#include "default_keymaps.h"
#include "environment/keymap_window.h"
#include "interfaces/keyboard_interface.h"

struct Gamepad : public QObject, NES::KeyboardInterface {
    Q_OBJECT

public slots:
    void key_pressed(Qt::Key key);

    void key_released(Qt::Key key);

    void show_editor() const;

signals:
    void state_changed(uint8_t);

public:
    ~Gamepad() override = default;

    void close();

    [[nodiscard]] uint8_t get_pressed_keys() const override;

    NES::KeyboardInterface *get_keyboard_interface();

    void load_player(NES::Players new_player);

private:
    std::atomic<uint8_t> pressed_keys_bitset{};
    QMap<Qt::Key, NES::Keys> key_to_btn;
    QMap<NES::Keys, Qt::Key> btn_to_key;
    KeymapWindow *keymap_editor = new KeymapWindow(key_to_btn, btn_to_key);
    NES::Players player = NES::Players::None;
};
