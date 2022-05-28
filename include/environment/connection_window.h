#pragma once

#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QTcpSocket>

class ConnectionWindow : public QWidget {
    Q_OBJECT
public:
    ConnectionWindow();
    ~ConnectionWindow() override = default;
    void closeEvent(QCloseEvent *event) override;

signals:
    void connect_btn_pressed(const QString &address, int);

private slots:
    void on_connect_btn_clicked();
    void on_cancel_btn_clicked();
    void enable_connect_btn();

private:
    QLineEdit *m_port_edit;
    QLineEdit *m_address_edit;
    QPushButton *m_connect_btn;
};