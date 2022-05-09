#include "environment/gamepad.h"
#include <QSettings>
#include "environment/keymap_window.h"

namespace NES {

uint8_t Gamepad::get_pressed_keys() const {
    return pressed_keys_bitset;
}

void Gamepad::key_pressed(Qt::Key key) {
    int value = static_cast<int>(key_to_btn.value(key, Keys::None));
    std::atomic<uint8_t> mask = (1u << value);  // (1 << 8) == 0
    pressed_keys_bitset |= mask;                // 2 operations but still ok
}

void Gamepad::key_released(Qt::Key key) {
    int value = static_cast<int>(key_to_btn.value(key, Keys::None));
    std::atomic<uint8_t> mask = ~(1u << value);  // ~(1 << 8) == 0xFF
    pressed_keys_bitset &= mask;                 // 2 operations but still ok
}

void Gamepad::show_editor() const {
    keymap_editor->show();
}

void Gamepad::load_player(Players new_player) {
    if (new_player == player) {
        return;
    }
    QSettings settings("NAD", "NADNES");  // TODO: make config file
    if (player != Players::None) {
        settings.beginGroup("keymap/player_" + QString::number(static_cast<int>(player)));
        for (int i = 0; i < BTN_COUNT; i++) {
            settings.setValue("key_" + QString::number(i), btn_to_key[NES::Keys(i)]);
        }
        settings.endGroup();
    }
    key_to_btn.clear();
    settings.beginGroup("keymap/player_" + QString::number(static_cast<int>(new_player)));
    for (int i = 0; i < BTN_COUNT; i++) {
        auto key = default_keymap[new_player][NES::Keys(i)];
        key = settings.value("key_" + QString::number(i), key).value<Qt::Key>();
        btn_to_key[NES::Keys(i)] = key;
        key_to_btn.insert(key, NES::Keys(i));
    }
    keymap_editor->m_player = new_player;
    keymap_editor->update_table();
    player = new_player;
}

}  // namespace NES