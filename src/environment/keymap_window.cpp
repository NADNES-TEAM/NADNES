#include "environment/keymap_window.h"
#include <QtUiTools>
#include <iostream>

namespace NES {

KeymapWindow::KeymapWindow(QMap<Qt::Key, NES::Keys> &key_to_btn_, const QString &title) : key_to_btn(key_to_btn_) {
    setWindowTitle(title);
    setFixedSize(509, 280);
    QFile file("../UI/keymap.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    QMetaObject::connectSlotsByName(this);
    m_sequence = findChild<QKeySequenceEdit *>("sequence_edit");
    m_table = findChild<QTableWidget *>("table_widget");
    for (auto &key : key_to_btn.keys()) {
        btn_to_key[key_to_btn[key]] = key;
        auto *cur = new QTableWidgetItem(QKeySequence(key).toString());
        cur->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(static_cast<int>(key_to_btn[key]), 1, cur);
    }
}

void KeymapWindow::on_restore_btn_clicked() {  // TODO
    m_table->item(0, 1)->setText("A");
    m_table->item(1, 1)->setText("S");
    m_table->item(2, 1)->setText("Tab");
    m_table->item(3, 1)->setText("Return");
    m_table->item(4, 1)->setText("Up");
    m_table->item(5, 1)->setText("Down");
    m_table->item(6, 1)->setText("Left");
    m_table->item(7, 1)->setText("Right");
}

void KeymapWindow::on_cancel_btn_clicked() {
    for (auto &btn : btn_to_key.keys()) {
        m_table->item(static_cast<int>(btn), 1)->setText(QKeySequence(btn_to_key[btn]).toString());
    }
    close();
}

void KeymapWindow::on_apply_btn_clicked() {
    QSettings settings("NAD", "NADNES");
    settings.beginGroup("keymap");
    for (int i = 0; i < 7; i++) {
        auto seq = QKeySequence(m_table->item(i, 1)->text());
        if (seq[0].key() != btn_to_key[NES::Keys(i)]) {
            key_to_btn.remove(btn_to_key[NES::Keys(i)]);
            key_to_btn.insert(seq[0].key(), NES::Keys(i));
            btn_to_key[NES::Keys(i)] = seq[0].key();
            settings.setValue("key_" + QString::number(i), seq[0].key());
        }
    }
    settings.endGroup();
    settings.sync();
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
        cur->setText(QKeySequence(btn_to_key[NES::Keys(cur->row())]).toString());
    } else {
        cur->setText(str);
    }
}

void KeymapWindow::on_table_widget_itemDoubleClicked() {
    m_sequence->setFocus();
}

}  // namespace NES