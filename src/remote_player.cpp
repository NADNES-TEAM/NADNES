#include "remote_player.h"
#include "colors_map.h"
#include <QDebug>

RemotePlayer::RemotePlayer(QObject *parent, QTcpSocket *socket_, size_t id_)
    : QObject(parent), btn(0) {
    TCP_socket = socket_;
    UDP_socket = new QUdpSocket(this);
    auto res = UDP_socket->bind(TCP_socket->localAddress(), 45454);
    qDebug() << res;
//  UDP_socket->connectToHost(TCP_socket->peerAddress(), 10001);
    id = id_;
    image.resize((NES::SCREEN_HEIGHT-1)*NES::SCREEN_WIDTH);
    connect(TCP_socket, SIGNAL(readyRead()), SLOT(data_arrived()));
    connect(TCP_socket, SIGNAL(disconnected()), SLOT(disconnect_wrapper()));
    stream.setDevice(socket_);
    stream.setVersion(QDataStream::Qt_4_0);
}

void RemotePlayer::set_pixel(int row, int column, NES::Color color) {
    if (0 < row && row < NES::SCREEN_HEIGHT && 0 < column && column <= NES::SCREEN_WIDTH) {
        image[(row - 1) * NES::SCREEN_WIDTH + column - 1] = bytes.at(color);
        // uint8_t byte = bytes.at(color);
        // stream << quint8(byte);
    }
}

uint8_t RemotePlayer::get_pressed_keys() const {
    return btn;
}

void RemotePlayer::data_arrived() {
    quint8 btn_qt = 0;
    while(TCP_socket->bytesAvailable()){
        TCP_socket->read(reinterpret_cast<char *>(&btn_qt),1);
    }
    btn = btn_qt;
}

void RemotePlayer::disconnect_wrapper() {
    emit disconnected(id);
}

void RemotePlayer::refresh_screen() {
//    qDebug() << "sent: ";
    auto res = UDP_socket->writeDatagram(image.data(),image.size(),TCP_socket->peerAddress(), 45454);
//    qDebug() << res << '\n';
    UDP_socket->flush();
}

NES::ScreenInterface *RemotePlayer::get_screen() {
    return dynamic_cast<NES::ScreenInterface *>(this);
}

NES::KeyboardInterface *RemotePlayer::get_keyboard() {
    return dynamic_cast<NES::KeyboardInterface *>(this);
}
