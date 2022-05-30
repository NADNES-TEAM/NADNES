#include "remote_emulator.h"
#include <QMessageBox>
#include <QSettings>
#include "colors_map.h"

RemoteEmulator::RemoteEmulator(QObject *parent, NES::ScreenInterface *screen_) : QObject(parent) {
    socket = new QTcpSocket(this);
    screen = screen_;
    connect(socket, SIGNAL(readyRead()), SLOT(data_arrived()));
    connect(socket, SIGNAL(disconnected()), SLOT(deleteLater()));
    connect(socket, &QAbstractSocket::errorOccurred, this, &RemoteEmulator::handle_error);
    stream.setDevice(socket);
    stream.setVersion(QDataStream::Qt_4_6);
    cur_x = cur_y = 1;
    connect(&connection_window,
            &ConnectionWindow::connect_btn_pressed,
            this,
            &RemoteEmulator::try_connect);
    read_settings();
    ask_for_reconnection();
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
    last_address = address;
    last_port = port;
    qDebug() << "Try connect: status = " << socket->state() << "\n";
}

void RemoteEmulator::handle_error(QAbstractSocket::SocketError error) {
    ask_for_reconnection("Following error occurred: " + socket->errorString() + "\n");
}

void RemoteEmulator::close() {
    write_settings();
    connection_window.close();
}

void RemoteEmulator::read_settings() {
    QSettings settings("NAD", "NADNES");
    settings.beginGroup("network/guest");
    last_address = settings.value("last_address").toString();
    last_port = settings.value("last_port").toInt();
    settings.endGroup();
}

void RemoteEmulator::write_settings() const {
    QSettings settings("NAD", "NADNES");
    settings.beginGroup("network/guest");
    settings.setValue("last_address", last_address);
    settings.setValue("last_port", last_port);
    settings.endGroup();
}

void RemoteEmulator::ask_for_reconnection(const QString &msg) {
    if (!last_address.isEmpty() && last_port != -1 &&
        QMessageBox::Yes ==
            QMessageBox::question(nullptr,
                                  "?",
                                  msg + "Would you like to reconnect to the last game?",
                                  (QMessageBox::Yes | QMessageBox::No),
                                  QMessageBox::Yes)) {
        try_connect(last_address, last_port);
    } else {
        connection_window.show();
    }
}
