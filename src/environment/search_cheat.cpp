#include "environment/search_cheat.h"

namespace NES::Cheating {

SearchCheat::SearchCheat(QWidget *parent) : QWidget(parent) {}
void SearchCheat::onNewButtonClick() {
    tableWidget->setRowCount(0);
    // TODO
}
Filter SearchCheat::create_filter() const {
    Filter filter;
    filter.byteCount = (twoBytes->isChecked() ? 2 : 1);
    if (eqRadio->isChecked()) {
        filter.compareType = CompareType::EQ;
    } else if (neqRadio->isChecked()) {
        filter.compareType = CompareType::NEQ;
    } else if (leRadio->isChecked()) {
        filter.compareType = CompareType::LE;
    } else if (leeqRadio->isChecked()) {
        filter.compareType = CompareType::LEEQ;
    } else if (grRadio->isChecked()) {
        filter.compareType = CompareType::GR;
    } else if (greqRadio->isChecked()) {
        filter.compareType = CompareType::GREQ;
    } else {
        assert(false);
    }
    filter.compareWith = ();
}

}