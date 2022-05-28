#include "remote_player.h"
#include "colors_map.h"

RemotePlayer::RemotePlayer(QTcpSocket *socket_,size_t id_) {
    socket = socket_;
    id = id_;
    connect(socket,SIGNAL(readyRead()),SLOT(data_arrived()));
    connect(socket,SIGNAL(disconnected()),SLOT(disconnect_wrapper()));
    stream.setDevice(socket_);
    stream.setVersion(QDataStream::Qt_4_0);
}


void RemotePlayer::set_pixel(int row, int column, NES::Color color) {
    uint8_t byte = bytes.at(color);
    if(!row && !column){
        byte+=0x80;
    }
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