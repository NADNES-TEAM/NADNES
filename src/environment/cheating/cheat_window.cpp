#include "environment/cheating/cheat_window.h"
#include <QFile>
#include <QUiLoader>
#include "environment/cheating/apply_cheat.h"
#include "environment/cheating/search_cheat.h"

namespace NES {
Cheating::CheatWindow::CheatWindow(QWidget *parent)
    : QWidget(parent) {
    QUiLoader loader;
    QFile fileMain("../UI/cheating/cheat_window.ui");  // just uis/... doesn't work
    fileMain.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileMain, this);
    tabWidget = findChild<QTabWidget*>("tabWidget");
    fileMain.close();
    setFixedSize(findChild<QWidget*>("Form")->size());

}

void Cheating::CheatWindow::init(Cheating::SearchCheat *search, Cheating::ApplyCheat *apply) {
    tabWidget->clear();
    tabWidget->addTab(search, "searchTab");
    tabWidget->addTab(apply, "apply");
}
}  // namespace NES
