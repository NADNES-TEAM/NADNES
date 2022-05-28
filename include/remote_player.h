#pragma once

#include <QObject>
#include <QTcpSocket>
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"

class RemotePlayer : public QObject, public NES::ScreenInterface, public NES::KeyboardInterface {
    Q_OBJECT

signals:
    void disconnected();

public slots:
    void flush();

public:
    RemotePlayer(std::unique_ptr<QTcpSocket> &&socket);
    void set_pixel(int row, int column, NES::Color color) override;

    // Vars
    std::unique_ptr<QTcpSocket> m_socket;
};
