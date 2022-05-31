#pragma once

#include <QTabWidget>
#include <QWidget>
#include "apply_cheat.h"
#include "search_cheat.h"

namespace NES::Cheating {
class CheatWindow : public QWidget {
    Q_OBJECT
public:
    explicit CheatWindow(QWidget *parent,
                         SearchCheat *search_cheat,
                         ApplyCheat *apply_cheat);
    ~CheatWindow() override = default;
public slots:

public:
    QTabWidget *tabWidget{};
};

}  // namespace NES::Cheating
