#pragma once
#include <QMap>
#include <QT>
#include <QWidget>
#include "default_keymaps.h"
#include "environment/keymap_window.h"
#include "interfaces/keyboard_interface.h"

namespace NES {

struct Gamepad : KeyboardInterface {
    QMap<Qt::Key, NES::Keys> key_to_btn;
    QMap<NES::Keys, Qt::Key> btn_to_key;
    std::atomic<uint8_t> pressed_keys_bitset{};
    KeymapWindow *keymap_editor = new KeymapWindow(key_to_btn, btn_to_key, player);
    Players player = Players::None;

    [[nodiscard]] uint8_t get_pressed_keys() const override;

    void key_pressed(Qt::Key key);

    void key_released(Qt::Key key);

    void load_player(Players new_player);

    void show_editor() const;
};

}  // namespace NES