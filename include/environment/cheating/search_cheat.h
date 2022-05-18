#pragma once

#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QWidget>
#include <QCheckBox>

namespace NES::Cheating {

//class CheatWindow;

class SearchCheat : public QWidget {
    Q_OBJECT
public:
    explicit SearchCheat(QWidget *parent = nullptr);
    ~SearchCheat() override = default;

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

