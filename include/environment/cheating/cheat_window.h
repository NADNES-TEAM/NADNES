#pragma once

#include <QTabWidget>
#include <QWidget>
#include "apply_cheat.h"
#include "search_cheat.h"

namespace NES::Cheating {
class CheatWindow : public QWidget {
    Q_OBJECT
public:
    explicit CheatWindow(QWidget *parent, Nes *nes);
    ~CheatWindow() override;
public slots:

private:
    QTabWidget *tabWidget{};
    SearchCheat *search_cheat{};
    ApplyCheat *apply_cheat{};
    CheatDbHandler cheatDbHandler{};
};

}  // namespace NES::Cheating
