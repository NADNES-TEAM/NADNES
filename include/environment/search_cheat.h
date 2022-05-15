#ifndef NADNES_SEARCH_CHEAT_H
#define NADNES_SEARCH_CHEAT_H

#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QWidget>
#include <QCheckBox>
#include "cheating/cheat_format.h"

namespace NES::Cheating {

//class CheatWindow;

class SearchCheat : public QWidget {
    Q_OBJECT
public:
    explicit SearchCheat(QWidget *parent = nullptr);
    ~SearchCheat() override = default;

    Filter create_filter() const;
public slots:
    void onNewButtonClick();

public:
    QPushButton *newButton{};
    QPushButton *filterButton{};
    QPushButton *exportButton{};

    QRadioButton *hexRadio{};
    QRadioButton *decRadio{};
    QRadioButton *twoBytes{};
    QRadioButton *oneByte{};
    QRadioButton *eqRadio{};
    QRadioButton *neqRadio{};
    QRadioButton *leRadio{};
    QRadioButton *leeqRadio{};
    QRadioButton *grRadio{};
    QRadioButton *greqRadio{};
    QLineEdit *compareWith{};

    QCheckBox *checkRom{};
    QCheckBox *checkRam{};

    QTableWidget *tableWidget{};
//    friend CheatWindow;
};

}  // namespace NES::Cheating

#endif  // NADNES_SEARCH_CHEAT_H
