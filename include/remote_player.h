#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QBuffer>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"
#include <QImage>
#include "nes_properties.h"

class RemotePlayer : public QObject, public NES::ScreenInterface, public NES::KeyboardInterface {
    Q_OBJECT

signals:
    void disconnected(size_t id);

public slots:
    void data_arrived();
    void disconnect_wrapper();

public:
    NES::ScreenInterface * get_screen();
    NES::KeyboardInterface * get_keyboard();
    RemotePlayer(QObject *parent, QTcpSocket *socket_, size_t id);
    void set_pixel(int row, int column, NES::Color color) override;
    uint8_t get_pressed_keys() const override;
    void refresh_screen() override;
    ~RemotePlayer() override = default;


private:
    QByteArray image;
    QDataStream stream;
    QUdpSocket *UDP_socket;
    QTcpSocket *TCP_socket;
    size_t id;
    uint8_t btn;
};
