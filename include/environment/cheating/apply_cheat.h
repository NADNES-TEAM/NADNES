#pragma once
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include "apply_cheat.h"


namespace NES::Cheating {
class CheatWindow;

class ApplyCheat : public QWidget {
    Q_OBJECT
public:
    explicit ApplyCheat(QWidget *parent = nullptr);
    ~ApplyCheat() override = default;
    void init();

private slots:


public:

    QPushButton *selectAll{};
    QPushButton *unselectAll{};
    QPushButton *applyButton{};
    QPushButton *loadCheatsButton{};

    QTableWidget *cheatTable{};
    friend CheatWindow;
};

}
