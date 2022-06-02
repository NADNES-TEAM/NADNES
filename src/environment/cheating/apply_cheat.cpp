#include "environment/cheating/apply_cheat.h"
#include <QFile>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QUiLoader>

namespace NES::Cheating {

ApplyCheat::ApplyCheat(QWidget *parent, NES::Nes *nes_) : QWidget(parent), nes(nes_) {
    QUiLoader loader;
    QFile fileApply("../UI/cheating/apply_cheat.ui");
    fileApply.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileApply, this);
    fileApply.close();
    connect(&qSignalMapperButton, &QSignalMapper::mappedInt, this, &ApplyCheat::on_ok_button);
    connect(&qSignalMapperDelete, &QSignalMapper::mappedInt, this, &ApplyCheat::on_del_button);
}

void ApplyCheat::init() {
    auto apply_cheat = this;
//    apply_cheat->selectAll = this->findChild<QPushButton *>("selectAll");
//    apply_cheat->unselectAll = this->findChild<QPushButton *>("unselectAll");
//    apply_cheat->applyButton = this->findChild<QPushButton *>("applyButton");
    apply_cheat->cheatTable = this->findChild<QTableWidget *>("cheatTable");
    apply_cheat->loadCheatsButton = this->findChild<QPushButton *>("loadCheats");
    connect(loadCheatsButton, SIGNAL(clicked()), this, SLOT(fill_table()));
    fill_table();
}

void ApplyCheat::fill_table() {
    setTo.clear();
    labels.clear();
    cheatTable->clear();
    uint64_t hash = nes->get_hash();
    names = cheatDbHandler->get_names(hash);
    cheats = cheatDbHandler->get_cheats(hash);
    cheatTable->setRowCount(names.size());
    int i = 0;
    for (const auto &it : names) {
        cheatTable->setItem(i, 0, new QTableWidgetItem(it));
        labels.push_back(it);

        auto qLineEdit = new QLineEdit();
        setTo.push_back(qLineEdit);
        cheatTable->setCellWidget(i, 1, qLineEdit);

        auto ok_button = new QPushButton(tr("Set"));
        connect(ok_button, SIGNAL(clicked()), &qSignalMapperButton, SLOT(map()));
        qSignalMapperButton.setMapping(ok_button, i);
        cheatTable->setCellWidget(i, 2, ok_button);

        auto del_button = new QPushButton(tr("Delete"));
        connect(del_button, SIGNAL(clicked()), &qSignalMapperDelete, SLOT(map()));
        qSignalMapperDelete.setMapping(del_button, i);
        cheatTable->setCellWidget(i, 3, del_button);

        i++;
    }
}

void ApplyCheat::on_ok_button(int j) {
    auto value = setTo[j]->text();
    auto addr = cheatDbHandler->get_addresses(cheats[j]);
    ParamsOfChange paramsOfChange;
    for (auto i : addr) {
        paramsOfChange.index = (i >> 18);
        paramsOfChange.place = (((i >> 1) & 1) ? Place::ROM : Place::RAM);
        paramsOfChange.byteCount = ((i & 1) ? ByteCount::TWO : ByteCount::ONE);
        paramsOfChange.data_in = setTo[j]->text().toLong();
        nes->change_memory_value(paramsOfChange);
    }
}

void ApplyCheat::on_del_button(int i) {
    cheatDbHandler->clear_name(labels[i]);
    fill_table();
    cheatDbHandler->save_data();
}

}  // namespace NES::Cheating
