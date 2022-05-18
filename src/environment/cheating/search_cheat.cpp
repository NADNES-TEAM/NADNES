#include "environment/cheating/search_cheat.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <fstream>

namespace NES::Cheating {

SearchCheat::SearchCheat(QWidget *parent) : QWidget(parent) {}

void SearchCheat::init() {
    connect(newButton, &QPushButton::clicked, this, &SearchCheat::onNewButtonClicked);
    connect(filterButton, &QPushButton::clicked, this, &SearchCheat::onFilterButtonClicked);
    connect(exportButton, &QPushButton::clicked, this, &SearchCheat::onExportButtonClicked);
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
    tableWidget->setRowCount(0);
    tableWidget->setRowCount(result.size());
    int i = 0;
    for (const auto &it : result) {
        tableWidget->setItem(i, 0, new QTableWidgetItem(tr("%1").arg(it.cur_value)));

        tableWidget->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(it.old_value)));
        tableWidget->setItem(
            i,
            2,
            new QTableWidgetItem(tr("%1").arg((it.place.id == 0 ? "RAM" : "ROM"))));
        i++;
    }
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
    auto file = std::ofstream(save_path.toStdString(), std::ios::binary);
    if (!file) {
        throw UnableToOpenFileError(save_path.toStdString());
    }
    save_cheat(file);
    qDebug() << "file write success\n";
}

}  // namespace NES::Cheating