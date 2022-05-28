#include "remote_player.h"
#include "colors_map.h"

RemotePlayer::RemotePlayer(std::unique_ptr<QTcpSocket> &&socket) : m_socket{std::move(socket)} {
}

void RemotePlayer::flush() {
    m_socket->flush();
}

void RemotePlayer::set_pixel(int row, int column, NES::Color color) {
    uint8_t byte = bytes.at(color);
    // TODO
}
