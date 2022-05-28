#include "environment/connection_window.h"
#include <QtUiTools>

ConnectionWindow::ConnectionWindow() {
    QFile file("../UI/player_select.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    QMetaObject::connectSlotsByName(this);

    m_connect_btn = findChild<QPushButton *>("connect_btn");
    m_connect_btn->setDisabled(true);

    m_port_edit = findChild<QLineEdit *>("port_edit");
    connect(m_port_edit, &QLineEdit::textChanged, this, &ConnectionWindow::enable_connect_btn);

    m_address_edit = findChild<QLineEdit *>("address_edit");
    connect(m_address_edit, &QLineEdit::textChanged, this, &ConnectionWindow::enable_connect_btn);
    m_address_edit->setValidator(new QIntValidator(1, 65535, this));
}

void ConnectionWindow::on_connect_btn_clicked() {
    emit connect_btn_pressed(m_address_edit->text(), m_port_edit->text().toInt());
}

void ConnectionWindow::enable_connect_btn() {
    m_connect_btn->setEnabled(!m_port_edit->text().isEmpty() && !m_address_edit->text().isEmpty());
}

void ConnectionWindow::on_cancel_btn_clicked() {
    m_port_edit->clear();
    m_address_edit->clear();
    m_connect_btn->setDisabled(true);
    close();
}

void ConnectionWindow::closeEvent(QCloseEvent *event) {
    on_cancel_btn_clicked();
}
