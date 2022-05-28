#include "remote_emulator.h"
#include "colors_map.h"
#include <string>
RemoteEmulator::RemoteEmulator(QTcpSocket *socket_, NES::ScreenInterface *screen_) {
    socket = socket_;
    screen = screen_;
    connect(socket,SIGNAL(readyRead()),SLOT(data_arrived()));
    stream.setDevice(socket_);
    stream.setVersion(QDataStream::Qt_4_0);
    cur_x=cur_y=0;
}
void RemoteEmulator::key_changed(uint8_t btn) {
    stream<<btn;
}
void RemoteEmulator::data_arrived() {
    QString data;
    stream>>data;
    for(const auto & byte : data){
        if(byte == char(0xFF)){
            (*screen).refresh_screen();
            cur_x=cur_y=0;
        }
        else {
            (*screen).set_pixel(cur_y, cur_x, colors[byte.unicode()]);
            cur_x++;
            cur_y += (cur_x == 0);
        }
    }
}