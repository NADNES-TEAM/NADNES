#include "remote_emulator.h"
#include <QMessageBox>
#include <QSettings>
#include "colors_map.h"
#include "config_reader.h"

RemoteEmulator::RemoteEmulator(QObject *parent, NES::ScreenInterface *screen_) : QObject(parent),timer(this) {
    socket = new QTcpSocket(this);
    screen = screen_;
    auto update_interval = config::get_value("client.force_update_interval_ms", 4);
    if(update_interval > -1) {
        timer.setInterval(std::chrono::milliseconds(update_interval));
        timer.callOnTimeout([this](){this->data_arrived();});
    }
    connect(socket, SIGNAL(readyRead()), SLOT(data_arrived()));

    connect(socket, &QAbstractSocket::errorOccurred, this, &RemoteEmulator::handle_error);
    connect(socket, &QAbstractSocket::connected, this, [this](){qDebug() << "start\n"; this->timer.start();});

    connection_window = new ConnectionWindow();
    connect(connection_window,
            &ConnectionWindow::connect_btn_pressed,
            this,
            &RemoteEmulator::try_connect);
    read_settings();
    ask_for_reconnection();
}

void RemoteEmulator::key_changed(uint8_t btn) {
    socket->write(reinterpret_cast<char *>(&btn),sizeof(uint8_t));
}

void RemoteEmulator::data_arrived() {
    int size_of_screen = (NES::SCREEN_HEIGHT - 1) * NES::SCREEN_WIDTH;
    while(socket->bytesAvailable() >= size_of_screen) {
        QByteArray data = socket->read(size_of_screen);
        for (int i = 0; i < NES::SCREEN_HEIGHT - 1; i++) {
            for (int j = 0; j < NES::SCREEN_WIDTH; j++) {
                screen->set_pixel(i + 1, j + 1, colors_to_bytes[data[i * NES::SCREEN_WIDTH + j]]);
            }
        }
    }
    screen->refresh_screen();
}

void RemoteEmulator::show_connection_window() {
    connection_window->show();
}

void RemoteEmulator::try_connect(const QString &address, int port) {
    socket->abort();
    socket->connectToHost(QHostAddress(address), port);
    qDebug() << "Connecting to: " << address << ' ' << port  << "\n";
    last_address = address;
    last_port = port;


}

void RemoteEmulator::handle_error(QAbstractSocket::SocketError error) {
    timer.stop();
    ask_for_reconnection("Following error occurred: " + socket->errorString() + "\n");
}

void RemoteEmulator::close() {
    socket->abort();
    timer.stop();
    write_settings();
    connection_window->close();
    connection_window->deleteLater();
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
        QTimer::singleShot(0, this, [this](){try_connect(last_address, last_port);});
    } else {
        connection_window->show();
    }
}
