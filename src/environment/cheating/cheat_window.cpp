#include "environment/cheating/cheat_window.h"
#include <QFile>
#include <QUiLoader>
#include "environment/cheating/apply_cheat.h"
#include "environment/cheating/search_cheat.h"

namespace NES {
Cheating::CheatWindow::CheatWindow(QWidget *parent,
                                   Cheating::SearchCheat *search_cheat,
                                   Cheating::ApplyCheat *apply_cheat)
    : QWidget(parent) {
    QUiLoader loader;
    QFile fileMain("../uis/cheating/cheat_window.ui");  // just uis/... doesn't work
    fileMain.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileMain, this);
    this->tabWidget = this->findChild<QTabWidget *>("tabWidget");
    fileMain.close();
    auto cheat_window = this;
    cheat_window->tabWidget->clear();
    cheat_window->tabWidget->addTab(search_cheat, "searchTab");
    cheat_window->tabWidget->addTab(apply_cheat, "applyTab");
}
}  // namespace NES
