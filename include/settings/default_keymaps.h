#pragma once

#include <QMap>

namespace NES {

constexpr int BTN_COUNT = 8;

enum class Players { None, SinglePlayer, Player1, Player2 };

enum class Keys : int { A, B, Select, Start, Up, Down, Left, Right, None };

const QMap<Players, QMap<NES::Keys, Qt::Key>> default_keymap = {
    {Players::SinglePlayer,
     {{Keys::A, Qt::Key::Key_A},
      {Keys::B, Qt::Key::Key_S},
      {Keys::Select, Qt::Key::Key_Tab},
      {Keys::Start, Qt::Key::Key_Return},
      {Keys::Up, Qt::Key::Key_Up},
      {Keys::Down, Qt::Key_Down},
      {Keys::Left, Qt::Key_Left},
      {Keys::Right, Qt::Key_Right}}},
    {Players::Player1,
     {{Keys::A, Qt::Key::Key_1},
      {Keys::B, Qt::Key::Key_2},
      {Keys::Select, Qt::Key::Key_Tab},
      {Keys::Start, Qt::Key::Key_Space},
      {Keys::Up, Qt::Key::Key_W},
      {Keys::Down, Qt::Key_S},
      {Keys::Left, Qt::Key_A},
      {Keys::Right, Qt::Key_D}}},
    {Players::Player2,
     {{Keys::A, Qt::Key::Key_K},
      {Keys::B, Qt::Key::Key_L},
      {Keys::Select, Qt::Key::Key_Backspace},
      {Keys::Start, Qt::Key::Key_Return},
      {Keys::Up, Qt::Key::Key_Up},
      {Keys::Down, Qt::Key_Down},
      {Keys::Left, Qt::Key_Left},
      {Keys::Right, Qt::Key_Right}}}

};
}  // namespace NES
