#pragma once

#include <QTabWidget>
#include <QWidget>
#include "apply_cheat.h"
#include "search_cheat.h"

namespace NES::Cheating {
class CheatWindow : public QWidget {
    Q_OBJECT
public:
    explicit CheatWindow(QWidget *parent);
    ~CheatWindow() override = default;
    void init(SearchCheat * search, ApplyCheat *apply);

private:
    QTabWidget *tabWidget{};
};

}  // namespace NES::Cheating
