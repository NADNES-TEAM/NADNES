#include "remote_emulator.h"
#include <QMessageBox>
#include "colors_map.h"

RemoteEmulator::RemoteEmulator(QTcpSocket *socket_, NES::ScreenInterface *screen_) {
    socket = new QTcpSocket(this);
    screen = screen_;
    connect(socket, SIGNAL(readyRead()), SLOT(data_arrived()));
    connect(socket, SIGNAL(disconnected()), SLOT(deleteLater));
    stream.setDevice(socket_);
    stream.setVersion(QDataStream::Qt_4_0);
    cur_x = cur_y = 0;
    connection_window = new ConnectionWindow();
    connect(connection_window, &ConnectionWindow::connect_btn_pressed, this, &RemoteEmulator::try_connect);
}

void RemoteEmulator::key_changed(uint8_t btn) {
    stream << btn;
}

void RemoteEmulator::data_arrived() {
    QString data;
    stream >> data;
    for (const auto &byte : data) {
        if (byte == char(0xFF)) {
            (*screen).refresh_screen();
            cur_x = cur_y = 0;
        } else {
            (*screen).set_pixel(cur_y, cur_x, colors[byte.unicode()]);
            cur_x++;
            cur_y += (cur_x == 0);
        }
    }
}

void RemoteEmulator::show_connection_window() {
    connection_window->show();
}

void RemoteEmulator::try_connect(const QString &address, int port) {
    socket->abort();
    socket->connectToHost(address, port);
}

void RemoteEmulator::handle_error(QAbstractSocket::SocketError error) {
    QMessageBox::critical(connection_window,
                          "Connection error",
                          "Following error occurred:" + socket->errorString(),
                          (QMessageBox::Ok));
}
