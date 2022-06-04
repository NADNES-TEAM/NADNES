#include "environment/cheating/cheat_window.h"
#include <QFile>
#include <QUiLoader>
#include "environment/cheating/apply_cheat.h"
#include "environment/cheating/search_cheat.h"
#include "nes_config.h"

namespace NES {
Cheating::CheatWindow::CheatWindow(QWidget *parent, NES::Nes *nes)
    : QWidget(parent) {

    search_cheat = new NES::Cheating::SearchCheat(nullptr, nes, &cheatDbHandler);
    apply_cheat = new NES::Cheating::ApplyCheat(nullptr, nes, &cheatDbHandler);
    search_cheat->init();
    apply_cheat->init();

    QUiLoader loader;
    QFile fileMain(config::get_value("ui_paths.cheat_window_path", defaults::ui_path_cheat_window).c_str());  // just uis/... doesn't work
    fileMain.open(QIODevice::ReadOnly | QIODevice::Text);
    loader.load(&fileMain, this);
    fileMain.close();
    tabWidget = findChild<QTabWidget*>("tabWidget");
    setFixedSize(findChild<QWidget*>("Form")->size());

    tabWidget->clear();
    tabWidget->addTab(search_cheat, "searchTab");
    tabWidget->addTab(apply_cheat, "applyTab");
}

Cheating::CheatWindow::~CheatWindow() {
    search_cheat->close();
    apply_cheat->close();
    search_cheat->deleteLater();
    apply_cheat->deleteLater();
}
}  // namespace NES
