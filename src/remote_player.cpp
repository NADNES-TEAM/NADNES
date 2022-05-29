#include "remote_player.h"
#include "colors_map.h"

RemotePlayer::RemotePlayer(QObject *parent, QTcpSocket *socket_, size_t id_): QObject(parent), btn(0) {
    socket = socket_;
    id = id_;
    connect(socket,SIGNAL(readyRead()),SLOT(data_arrived()));
    connect(socket,SIGNAL(disconnected()),SLOT(disconnect_wrapper()));
    stream.setDevice(socket_);
    stream.setVersion(QDataStream::Qt_4_0);
}


void RemotePlayer::set_pixel(int row, int column, NES::Color color) {
    uint8_t byte = bytes.at(color);
    stream<<byte;
}
uint8_t RemotePlayer::get_pressed_keys() const {
    return btn;
}
void RemotePlayer::data_arrived() {
    stream>>btn;
}
void RemotePlayer::disconnect_wrapper() {
    emit(disconnected(id));
}
void RemotePlayer::refresh_screen() {
    uint8_t data = 0xFF;
    stream>>data;
}
NES::ScreenInterface *RemotePlayer::get_screen(){
    return dynamic_cast<NES::ScreenInterface *>(this);
}
NES::KeyboardInterface *RemotePlayer::get_keyboard() {
    return dynamic_cast<NES::KeyboardInterface *>(this);
}
