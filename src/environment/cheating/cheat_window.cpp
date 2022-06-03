#include "environment/cheating/cheat_window.h"
#include <QFile>
#include <QUiLoader>
#include "environment/cheating/apply_cheat.h"
#include "environment/cheating/search_cheat.h"

namespace NES {
Cheating::CheatWindow::CheatWindow(QWidget *parent, NES::Nes *nes)
    : QWidget(parent) {

    search_cheat = new NES::Cheating::SearchCheat(nullptr, nes);
    apply_cheat = new NES::Cheating::ApplyCheat(nullptr, nes);

    search_cheat->cheatDbHandler = &cheatDbHandler;
    apply_cheat->cheatDbHandler = &cheatDbHandler;
    search_cheat->init();
    apply_cheat->init();

    QUiLoader loader;
    QFile fileMain("../UI/cheating/cheat_window.ui");  // just uis/... doesn't work
    fileMain.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileMain, this);
    this->tabWidget = this->findChild<QTabWidget *>("tabWidget");
    fileMain.close();
    auto cheat_window = this;
    cheat_window->tabWidget->clear();
    cheat_window->tabWidget->addTab(search_cheat, "searchTab");
    cheat_window->tabWidget->addTab(apply_cheat, "applyTab");
}

Cheating::CheatWindow::~CheatWindow() {
    search_cheat->close();
    apply_cheat->close();
    search_cheat->deleteLater();
    apply_cheat->deleteLater();
}
}  // namespace NES
