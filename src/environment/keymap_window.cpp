#include "environment/keymap_window.h"
#include <QtUiTools>
#include <iostream>

namespace NES {

KeymapWindow::KeymapWindow(QMap<Qt::Key, NES::Keys> &key_to_btn_) : key_to_btn(key_to_btn_) {
    setFixedSize(509, 280);
    QFile file("../keymap.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    QMetaObject::connectSlotsByName(this);

    table = findChild<QTableWidget *>("table");
    for (auto &key : key_to_btn.keys()) {
        btn_to_key[key_to_btn[key]] = key;
        auto *cur = new QTableWidgetItem(QKeySequence(key).toString());
        cur->setTextAlignment(Qt::AlignCenter);
        table->setItem(static_cast<int>(key_to_btn[key]), 1, cur);
    }
}

void KeymapWindow::on_table_cellChanged(int row, int col) {
    auto item = table->item(row, col);
    auto seq = QKeySequence(item->text()).toString();
    if (seq.isEmpty() || seq.count('+') != 0) {
        QMessageBox::warning(this,
                             "Bad key",
                             "Only single character bindings are allowed!",
                             (QMessageBox::Ok));
        item->setText(QKeySequence(btn_to_key[NES::Keys(row)]).toString());
    }
}

void KeymapWindow::on_restore_btn_clicked() {  // TODO
    table->item(0, 1)->setText("A");
    table->item(1, 1)->setText("S");
    table->item(2, 1)->setText("Tab");
    table->item(3, 1)->setText("Return");
    table->item(4, 1)->setText("Up");
    table->item(5, 1)->setText("Down");
    table->item(6, 1)->setText("Left");
    table->item(7, 1)->setText("Right");
}

void KeymapWindow::on_cancel_btn_clicked() {
    for (auto &btn : btn_to_key.keys()) {
        table->item(static_cast<int>(btn), 1)->setText(QKeySequence(btn_to_key[btn]).toString());
    }
    close();
}

void KeymapWindow::on_apply_btn_clicked() {
    QSettings settings("NAD", "NADNES");
    settings.beginGroup("keymap");
    for (int i = 0; i < 7; i++) {
        auto seq = QKeySequence(table->item(i, 1)->text());
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

}  // namespace NES