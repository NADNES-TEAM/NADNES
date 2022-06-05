#pragma once

#include <QBuffer>
#include <QImage>
#include <QObject>
#include <QTcpSocket>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"
#include "nes_config.h"
#include "nes_properties.h"

class RemotePlayer : public QObject, public NES::ScreenInterface, public NES::KeyboardInterface {
    Q_OBJECT

signals:
    void disconnected(size_t id);

public slots:
    void data_arrived();
    void disconnect_wrapper();

public:
    NES::ScreenInterface *get_screen();
    NES::KeyboardInterface *get_keyboard();
    RemotePlayer(QObject *parent, QTcpSocket *socket_, int id);
    void set_pixel(int row, int column, NES::Color color) override;
    uint8_t get_pressed_keys() const override;
    void refresh_screen() override;
    ~RemotePlayer() override = default;

private:
    const int send_rate =
        std::max(1, NES::PPU_VERTICAL_FRAME_RATE_FREQ_HZ / Config::get_value().server_send_rate);
    QByteArray image;
    QTcpSocket *socket;
    int id;
    uint8_t btn;
    uint64_t frame_count = 0;
};
