#include "remote_emulator.h"
#include <QMessageBox>
#include "colors_map.h"
#include "nes_properties.h"

RemoteEmulator::RemoteEmulator(QObject *parent, NES::ScreenInterface *screen_)
    : QObject(parent) {
    socket = new QTcpSocket(this);
    screen = screen_;
    connect(socket, SIGNAL(readyRead()), SLOT(data_arrived()));
    connect(socket, SIGNAL(disconnected()), SLOT(deleteLater()));
    connect(socket, &QAbstractSocket::errorOccurred,
            this, &RemoteEmulator::handle_error);
    stream.setDevice(socket);
    stream.setVersion(QDataStream::Qt_4_6);
    cur_x = cur_y = 1;
    connect(&connection_window,
            &ConnectionWindow::connect_btn_pressed,
            this,
            &RemoteEmulator::try_connect);
}

void RemoteEmulator::key_changed(uint8_t btn) {
    stream << quint8(btn);
}

void RemoteEmulator::data_arrived() {
    while (!stream.atEnd()) {
        quint8 byte_qt;
        stream >> byte_qt;
        uint8_t byte = byte_qt;
        if (byte == 0xFF) {
            screen->refresh_screen();
            cur_x = cur_y = 0;
        } else {
            screen->set_pixel(cur_y + 1, cur_x + 1, colors[byte]);
            cur_x++;
            cur_y += (cur_x == 0);
        }
    }
}

void RemoteEmulator::show_connection_window() {
    connection_window.show();
}

void RemoteEmulator::try_connect(const QString &address, int port) {
    socket->abort();
    socket->connectToHost(address, port);
    qDebug() << "Try connect: status = " << socket->state() << "\n";
}

void RemoteEmulator::handle_error(QAbstractSocket::SocketError error) {
    QMessageBox::critical(&connection_window,
                          "Connection error",
                          "Following error occurred:" + socket->errorString(),
                          (QMessageBox::Ok));
}

void RemoteEmulator::close() {
    connection_window.close();
}
