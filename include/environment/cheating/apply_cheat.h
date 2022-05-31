#pragma once
#include <QPushButton>
#include <QSignalMapper>
#include <QTableWidget>
#include <QWidget>
#include "apply_cheat.h"
#include "cheat_db_handler2.h"

namespace NES::Cheating {
class ApplyCheat : public QWidget {
    Q_OBJECT
public:
    explicit ApplyCheat(QWidget *parent, NES::Nes *nes_);
    ~ApplyCheat() override = default;
    void init();

public slots:
    void fill_table();
    void on_ok_button(int i);
    void on_del_button(int i);

public:
//    QPushButton *selectAll{};
//    QPushButton *unselectAll{};
//    QPushButton *applyButton{};
    QPushButton *loadCheatsButton{};

    QTableWidget *cheatTable{};
    CheatDbHandler *cheatDbHandler{};
    QSignalMapper qSignalMapperButton;
    QSignalMapper qSignalMapperDelete;

private:
    NES::Nes *nes{};
    std::vector<QString> names;
    std::vector<QLineEdit *> setTo;
    std::vector<QString> labels;
    std::vector<quint64> cheats;
};

}  // namespace NES::Cheating
