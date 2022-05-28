#pragma once

#include <QObject>
#include <QTcpSocket>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"

class RemotePlayer : public QObject, public NES::ScreenInterface, public NES::KeyboardInterface {
    Q_OBJECT

signals:
    void disconnected(size_t id);

public slots:
    void data_arrived();
    void disconnect_wrapper();

public:
    RemotePlayer(QTcpSocket *socket_, size_t id);
    void set_pixel(int row, int column, NES::Color color) override;
    uint8_t get_pressed_keys() const override;
    ~RemotePlayer() override = default;

private:
    QDataStream stream;
    QTcpSocket *socket;
    size_t id;
    uint8_t btn;
};