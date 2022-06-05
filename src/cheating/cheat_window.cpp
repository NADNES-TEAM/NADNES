#include "cheating/cheat_window.h"
#include <QFile>
#include <QUiLoader>
#include "cheating/apply_cheat.h"
#include "cheating/search_cheat.h"
#include "settings/nes_config.h"

namespace NES {
Cheating::CheatWindow::CheatWindow(QWidget *parent, NES::Nes *nes) : QWidget(parent) {
    search_cheat = new NES::Cheating::SearchCheat(nullptr, nes, &cheatDbHandler);
    apply_cheat = new NES::Cheating::ApplyCheat(nullptr, nes, &cheatDbHandler);
    search_cheat->init();
    apply_cheat->init();

    QUiLoader loader;
    QFile fileMain(Config::get_value().ui_path_cheat_window.c_str());  // just uis/... doesn't work
    fileMain.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileMain, this);
    fileMain.close();
    tabWidget = findChild<QTabWidget *>("tabWidget");
    setFixedSize(findChild<QWidget *>("Form")->size());

    tabWidget->clear();
    tabWidget->addTab(search_cheat, "Search");
    tabWidget->addTab(apply_cheat, "Apply");
}

Cheating::CheatWindow::~CheatWindow() {
    search_cheat->close();
    apply_cheat->close();
    search_cheat->deleteLater();
    apply_cheat->deleteLater();
}
}  // namespace NES
