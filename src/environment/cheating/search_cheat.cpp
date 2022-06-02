#include "environment/cheating/search_cheat.h"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QRegularExpression>
#include <QUiLoader>
#include <iostream>

namespace NES::Cheating {

SearchCheat::SearchCheat(QWidget *parent, NES::Nes *nes) : QWidget(parent), nes(nes) {
    QUiLoader loader;

    QFile fileSearch("../UI/cheating/search_cheat.ui");
    fileSearch.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileSearch, this);
    fileSearch.close();
}

void SearchCheat::init() {
    auto search_cheat = this;

    search_cheat->newButton = search_cheat->findChild<QPushButton *>("newButton");
    search_cheat->filterButton = search_cheat->findChild<QPushButton *>("filterButton");
    search_cheat->exportButton = search_cheat->findChild<QPushButton *>("exportButton");
    search_cheat->hexRadio = search_cheat->findChild<QRadioButton *>("hexRadio");
    search_cheat->decRadio = search_cheat->findChild<QRadioButton *>("decRadio");
    search_cheat->twoBytes = search_cheat->findChild<QRadioButton *>("twoBytes");
    search_cheat->oneByte = search_cheat->findChild<QRadioButton *>("oneByte");
    search_cheat->eqRadio = search_cheat->findChild<QRadioButton *>("eqRadio");
    search_cheat->all = search_cheat->findChild<QRadioButton *>("all");
    search_cheat->leRadio = search_cheat->findChild<QRadioButton *>("leRadio");
    search_cheat->leeqRadio = search_cheat->findChild<QRadioButton *>("leeqRadio");
    search_cheat->grRadio = search_cheat->findChild<QRadioButton *>("grRadio");
    search_cheat->greqRadio = search_cheat->findChild<QRadioButton *>("greqRadio");
    search_cheat->decreased = search_cheat->findChild<QRadioButton *>("decreased");
    search_cheat->increased = search_cheat->findChild<QRadioButton *>("increased");
    search_cheat->decreased_or_save = search_cheat->findChild<QRadioButton *>("decreased_save");
    search_cheat->increased_or_save = search_cheat->findChild<QRadioButton *>("increased_save");
    search_cheat->save = search_cheat->findChild<QRadioButton *>("save");
    search_cheat->changed = search_cheat->findChild<QRadioButton *>("changed");
    search_cheat->checkRam = search_cheat->findChild<QCheckBox *>("checkRAM");
    search_cheat->checkRom = search_cheat->findChild<QCheckBox *>("checkROM");
    search_cheat->tableWidget = search_cheat->findChild<QTableWidget *>("tableWidget");
    search_cheat->compareWith = search_cheat->findChild<QLineEdit *>("compareWith");

    connect(newButton, &QPushButton::clicked, this, &SearchCheat::onNewButtonClicked);
    connect(filterButton, &QPushButton::clicked, this, &SearchCheat::onFilterButtonClicked);
    connect(exportButton, &QPushButton::clicked, this, &SearchCheat::onExportButtonClicked);
    connect(tableWidget, &QTableWidget::cellChanged, this, &SearchCheat::onCellChanged);
    connect(&qSignalMapper, SIGNAL(mappedInt(int)), this, SLOT(handleButton(int)));
}

void SearchCheat::onNewButtonClicked() {
    if (nes == nullptr) {
        return;
        // TODO
    }
    if (!checkRam->isChecked() && !checkRom->isChecked()) {
        //        QMessageBox message;
        //        message.setText(tr("Please, choose at least one place to search"));
        //        message.setStandardButtons(QMessageBox::Ok);
        //        message.exec();
        // TODO
        return;
    }

    // TODO check that params are correct
    result = nes->search(getParams(), result);
    fillTable();
}

void SearchCheat::some_slot() {
    std::cout << "cout" << std::endl;
    qDebug() << "qDebug\n";
}
void SearchCheat::onFilterButtonClicked() {
    if (nes == nullptr) {
        return;
        // TODO
    }
    if (!checkRam->isChecked() && !checkRom->isChecked()) {
        // TODO
        return;
    }

    // TODO check that params are correct
    auto params = getParams();
    params.is_initial = false;
    result = nes->search(params, result);
    fillTable();
}

ParamsOfSearch SearchCheat::getParams() const {
    ParamsOfSearch paramsOfSearch;
    paramsOfSearch.place.id = int(checkRam->isChecked()) + 2 * int(checkRom->isChecked()) - 1;
    paramsOfSearch.is_initial = true;
    paramsOfSearch.byteCount = ByteCount{size_t(oneByte->isChecked() ? 1 : 2)};
    if (save->isChecked()) {
        paramsOfSearch.event = Action::save;
    } else if (increased->isChecked()) {
        paramsOfSearch.event = Action::encrease;
    } else if (decreased->isChecked()) {
        paramsOfSearch.event = Action::decrease;
    } else if (increased_or_save->isChecked()) {
        paramsOfSearch.event = Action::encrease_or_save;
    } else if (decreased_or_save->isChecked()) {
        paramsOfSearch.event = Action::decrease_or_save;
    } else if (changed->isChecked()) {
        paramsOfSearch.event = Action::changed;
    } else if (leRadio->isChecked()) {
        paramsOfSearch.event = Action::l_num;
    } else if (grRadio->isChecked()) {
        paramsOfSearch.event = Action::g_num;
    } else if (leeqRadio->isChecked()) {
        paramsOfSearch.event = Action::leq_num;
    } else if (greqRadio->isChecked()) {
        paramsOfSearch.event = Action::geq_num;
    } else if (eqRadio->isChecked()) {
        paramsOfSearch.event = Action::eq_num;
    } else if (all->isChecked()) {
        paramsOfSearch.event = Action::all;
    }

    if (decRadio->isChecked()) {
        paramsOfSearch.data_in = std::stoi(compareWith->text().toStdString());
    } else {
        std::string s = compareWith->text().toStdString();
        std::reverse(s.begin(), s.end());
        paramsOfSearch.data_in = std::stoi(s, nullptr, 16);
    }
    return paramsOfSearch;
}
void SearchCheat::fillTable() {
    disconnect(tableWidget, &QTableWidget::cellChanged, this, &SearchCheat::onCellChanged);

    tableWidget->setRowCount(0);
    tableWidget->setRowCount(result.size());
    int i = 0;
    for (const auto &it : result) {
        QTableWidgetItem *item;
        item = new QTableWidgetItem(tr("%1").arg(it.cur_value));
        tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem(tr("%1").arg(it.old_value));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(i, 1, item);

        item = new QTableWidgetItem(tr("%1").arg((it.place.id == 0 ? "RAM" : "ROM")));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem(tr("%1").arg(it.address));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(i, 3, item);

        auto widget = new QPushButton(tr("Del"));
        connect(widget, SIGNAL(clicked()), &qSignalMapper, SLOT(map()));
        qSignalMapper.setMapping(widget, i);
        tableWidget->setCellWidget(i, 4, widget);

        i++;
    }
    connect(tableWidget, &QTableWidget::cellChanged, this, &SearchCheat::onCellChanged);
}
void SearchCheat::onExportButtonClicked() {
    if (!nes) {
        return;
    }
    QFile defaultSaveFile("../UI/cheating/save_cheat_window.ui");
    QUiLoader qUiLoader;
    defaultSaveFile.open(QIODevice::ReadOnly | QIODevice::Text);
    save_default = new QWidget();
    qUiLoader.load(&defaultSaveFile, save_default);
    defaultSaveFile.close();
    buttonBox = save_default->findChild<QDialogButtonBox *>("buttonBox");
    defaultSaveEdit = save_default->findChild<QLineEdit *>("lineEdit");
    QRegularExpression re("^[-_a-zA-Z]+$");
    auto validator = new QRegularExpressionValidator(re, this);
    defaultSaveEdit->setValidator(validator);
    save_default->show();
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(onOkButtonClicked()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(closeDialog()));
}

void SearchCheat::onOkButtonClicked() {
    cheatDbHandler->clear_name(defaultSaveEdit->text());
    auto hash = nes->get_hash();
//    qDebug() << "hash: " << hash;
    auto cheat_num = cheatDbHandler->add_name(defaultSaveEdit->text());
//    qDebug() << "cheat_num: " << cheat_num;
    cheatDbHandler->add_cheat(hash, cheat_num);
//    qDebug() << "add_cheat: " << hash << " " << cheat_num;
    for (const auto &it : result) {
        quint64 address = (quint64(it.address) << 18) + (it.place.id == Place::RAM.id ? 0 : 2) +
                          (this->oneByte->isChecked() ? 0 : 1);
//        qDebug() << "address: " << address;
        cheatDbHandler->add_address(cheat_num, address);
    }
    closeDialog();
}

void SearchCheat::closeDialog() const {
    save_default->deleteLater();
}
void SearchCheat::onCellChanged(int row, int column) {
    if (column != 0) {
        qDebug() << column << "\n";
        return;
    }
    ParamsOfChange paramsOfChange;
    paramsOfChange.place.id = result[row].place.id;
    // TODO save params
    paramsOfChange.byteCount = ByteCount{size_t(oneByte->isChecked() ? 1 : 2)};
    auto s = tableWidget->itemAt(row, column)->text().toStdString();
    if (hexRadio->isChecked()) {
        std::reverse(s.begin(), s.end());
        paramsOfChange.data_in = std::stoi(s, nullptr, 16);
    } else {
        paramsOfChange.data_in = std::stoi(s);
    }
    result[row].old_value = result[row].cur_value;
    result[row].cur_value = paramsOfChange.data_in;
    paramsOfChange.index = result[row].address;
    Nes::change_memory_value(paramsOfChange);
}

void SearchCheat::handleButton(int id) {
    //    qDebug() << id;
    result.erase(result.begin() + id);
    fillTable();
}

}  // namespace NES::Cheating