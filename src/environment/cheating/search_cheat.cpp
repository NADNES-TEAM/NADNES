#include "environment/cheating/search_cheat.h"

namespace NES::Cheating {

SearchCheat::SearchCheat(QWidget *parent) : QWidget(parent) {}
void SearchCheat::onNewButtonClick() {
    tableWidget->setRowCount(0);
    // TODO
}

}