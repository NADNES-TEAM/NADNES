#include "environment/keymap_window.h"
#include <QtUiTools>

KeymapWindow::KeymapWindow(QMap<Qt::Key, NES::Keys> &key_to_btn_,
                           QMap<NES::Keys, Qt::Key> &btn_to_key_)
    : m_key_to_btn(key_to_btn_), m_btn_to_key(btn_to_key_){
    //    setWindowTitle(title);
    setFixedSize(509, 280);  // TODO: config?
    QFile file("../UI/keymap.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    QMetaObject::connectSlotsByName(this);
    m_sequence = findChild<QKeySequenceEdit *>("sequence_edit");
    m_table = findChild<QTableWidget *>("table_widget");
    update_table();
}

void KeymapWindow::on_restore_btn_clicked() {
    for (int i = 0; i < NES::BTN_COUNT; i++) {
        auto cur = QKeySequence(NES::default_keymap[m_player][NES::Keys(i)]).toString();
        m_table->item(i, 1)->setText(cur);
    }
}

void KeymapWindow::on_cancel_btn_clicked() {
    update_table();
    close();
}

void KeymapWindow::on_apply_btn_clicked() {
    for (int i = 0; i < NES::BTN_COUNT; i++) {
        auto seq = QKeySequence(m_table->item(i, 1)->text());
        if (seq[0].key() == m_btn_to_key[NES::Keys(i)]) {
            continue;
        }
        m_key_to_btn.remove(m_btn_to_key[NES::Keys(i)]);
        m_key_to_btn.insert(seq[0].key(), NES::Keys(i));
        m_btn_to_key[NES::Keys(i)] = seq[0].key();
    }
}

void KeymapWindow::on_save_btn_clicked() {
    on_apply_btn_clicked();
    close();
}

void KeymapWindow::on_sequence_edit_editingFinished() {
    if (m_table->selectedItems().isEmpty()) {
        return;
    }
    auto cur = m_table->selectedItems()[0];
    auto seq = m_sequence->keySequence();
    auto str = seq.toString();
    if (seq.count() != 1 || str.count("+") != 0 || str.isEmpty()) {
        QMessageBox::warning(this,
                             "Bad key",
                             "Only single character bindings are allowed!",
                             (QMessageBox::Ok));
        cur->setText(QKeySequence(m_btn_to_key[NES::Keys(cur->row())]).toString());
    } else {
        cur->setText(str);
    }
}

void KeymapWindow::on_table_widget_itemDoubleClicked() {
    m_sequence->setFocus();
}

void KeymapWindow::update_table() {
    for (int i = 0; i < NES::BTN_COUNT; i++) {
        m_table->item(i, 1)->setText(QKeySequence(m_btn_to_key[NES::Keys(i)]).toString());
    }
}