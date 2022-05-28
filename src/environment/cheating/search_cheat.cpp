#include "environment/cheating/search_cheat.h"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QUiLoader>
#include <fstream>
#include <iostream>

namespace NES::Cheating {

SearchCheat::SearchCheat(QWidget *parent) : QWidget(parent) {}

void SearchCheat::init() {
    connect(newButton, &QPushButton::clicked, this, &SearchCheat::onNewButtonClicked);
    connect(filterButton, &QPushButton::clicked, this, &SearchCheat::onFilterButtonClicked);
    connect(exportButton, &QPushButton::clicked, this, &SearchCheat::onExportButtonClicked);
    connect(tableWidget, &QTableWidget::cellChanged, this, &SearchCheat::onCellChanged);
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
        i++;
    }
    connect(tableWidget, &QTableWidget::cellChanged, this, &SearchCheat::onCellChanged);
}
void SearchCheat::onExportButtonClicked() {
    if (!nes) {
        return;
    }
    QString save_path =
        QFileDialog::getSaveFileName(this, "Select save file", "", "NADNES cheats (*.cheat)");
    if (save_path.isEmpty()) {
        return;
    }
    file_name = save_path.toStdString();
    QFile defaultSaveFile("../uis/cheating/save_cheat_window.ui");
    QUiLoader qUiLoader;
    defaultSaveFile.open(QIODevice::ReadOnly | QIODevice::Text);
    save_default = new QWidget();
    qUiLoader.load(&defaultSaveFile, save_default);
    defaultSaveFile.close();
    save_default->show();
    buttonBox = save_default->findChild<QDialogButtonBox *>("buttonBox");
    defaultSaveEdit = save_default->findChild<QLineEdit *>("lineEdit");
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(onOkButtonClicked()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(closeDialog()));
    save_default->show();
}

void SearchCheat::onOkButtonClicked() {
    auto file = std::ofstream(file_name, std::ios::binary);
    if (!file) {
        throw UnableToOpenFileError(file_name);
    }
    save_cheat(file);
    qDebug() << "file write success\n";
    closeDialog();
}

void SearchCheat::closeDialog() {
    save_default->close();
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
    auto s =
        tableWidget->itemAt(row, column)->text().toStdString();
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

}  // namespace NES::Cheating