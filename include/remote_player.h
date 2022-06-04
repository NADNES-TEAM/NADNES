#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QBuffer>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"
#include <QImage>
#include "nes_properties.h"
#include "config_reader.h"

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
    RemotePlayer(QObject *parent, QTcpSocket *socket_, int id);
    void set_pixel(int row, int column, NES::Color color) override;
    uint8_t get_pressed_keys() const override;
    void refresh_screen() override;
    ~RemotePlayer() override = default;


private:
    const int send_rate = NES::PPU_VERTICAL_FRAME_RATE_FREQ_HZ /
                          config::get_value("server.send_framerate", 20);
    QByteArray image;
    QTcpSocket *socket;
    int id;
    uint8_t btn;
    uint64_t frame_count = 0;
};
