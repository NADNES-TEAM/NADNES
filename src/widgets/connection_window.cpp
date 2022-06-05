#include "widgets/connection_window.h"
#include <QtUiTools>
#include "settings/nes_config.h"

ConnectionWindow::ConnectionWindow() {
    QFile file(Config::get_value().ui_path_connect_window.c_str());
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    file.close();
    QMetaObject::connectSlotsByName(this);
    setFixedSize(findChild<QWidget *>("Form")->size());

    m_connect_btn = findChild<QPushButton *>("connect_btn");
    m_connect_btn->setDisabled(true);

    m_port_edit = findChild<QLineEdit *>("port_edit");
    connect(m_port_edit, &QLineEdit::textChanged, this, &ConnectionWindow::enable_connect_btn);
    m_port_edit->setValidator(new QIntValidator(1, 65535, this));

    m_address_edit = findChild<QLineEdit *>("address_edit");
    connect(m_address_edit, &QLineEdit::textChanged, this, &ConnectionWindow::enable_connect_btn);
}

void ConnectionWindow::on_connect_btn_clicked() {
    emit connect_btn_pressed(m_address_edit->text(), m_port_edit->text().toInt());
    hide();
}

void ConnectionWindow::enable_connect_btn() {
    m_connect_btn->setEnabled(!m_port_edit->text().isEmpty() && !m_address_edit->text().isEmpty());
}

void ConnectionWindow::on_cancel_btn_clicked() {
    //    m_port_edit->clear();
    //    m_address_edit->clear();
    m_connect_btn->setDisabled(true);
    //    qDebug() << "canceled\n";
    hide();
}

void ConnectionWindow::closeEvent(QCloseEvent *event) {
    on_cancel_btn_clicked();
    //    qDebug() << "closed\n";
}
