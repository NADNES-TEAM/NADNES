#include "environment/player_select_window.h"
#include <QMessageBox>
#include <QtUiTools>

PlayerSelectWindow::PlayerSelectWindow() {
    QFile file("../UI/player_select.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    QMetaObject::connectSlotsByName(this);

    m_player1_select = findChild<QComboBox *>("player1_select");
    m_player2_select = findChild<QComboBox *>("player2_select");
}

void PlayerSelectWindow::on_ok_btn_clicked() {
    int player1_cur = m_player1_select->currentIndex();
    int player2_cur = m_player2_select->currentIndex();
    if (player1_cur == player2_cur && m_player2_select->isEnabled()) {
        QMessageBox::warning(this, "Players matched", "Players must differ!", (QMessageBox::Ok));
        return;
    }
    m_player1_index = player1_cur;
    m_player2_index = player2_cur;
    emit selected(m_player1_index, m_player2_select->isEnabled() ? player2_cur : -1);
    close();
}

void PlayerSelectWindow::on_cancel_btn_clicked() {
    m_player1_select->setCurrentIndex(m_player1_index);
    m_player2_select->setCurrentIndex(m_player2_index);
    close();
}

void PlayerSelectWindow::add_player(int index, const QString &name) {
    m_player1_select->insertItem(index, name);
    m_player2_select->insertItem(index, name);
}

void PlayerSelectWindow::remove_player(int index) {
    m_player1_select->removeItem(index);
    m_player2_select->removeItem(index);
}
