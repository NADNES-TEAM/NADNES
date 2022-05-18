#pragma once
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>


namespace NES::Cheating {
class CheatWindow;

class ApplyCheat : public QWidget {
    Q_OBJECT
public:
    explicit ApplyCheat(QWidget *parent = nullptr);
    ~ApplyCheat() override = default;
    void init();

private slots:
//    void onNewClicked();


public:

    QPushButton *selectAll{};
    QPushButton *unselectAll{};
    QPushButton *applyButton{};

    QTableWidget *cheatTable{};
    friend CheatWindow;
};

}
