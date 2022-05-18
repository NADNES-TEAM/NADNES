#pragma once

#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QWidget>
#include <QCheckBox>
#include "search.h"
#include "nes.h"

namespace NES::Cheating {

//class CheatWindow;

class SearchCheat : public QWidget {
    Q_OBJECT
public:
    explicit SearchCheat(QWidget *parent = nullptr);
    ~SearchCheat() override = default;

    void init();
    [[nodiscard]] ParamsOfSearch getParams() const;
    void fillTable();

    void save_cheat(std::ostream &file) {
        size_t n = result.size();
        file.write(reinterpret_cast<char *>(&n), sizeof(n));
        file.write(reinterpret_cast<char *>(&result[0]), n * sizeof(ResultRaw));
    }

public slots:
    void onNewButtonClicked();
    void onFilterButtonClicked();
    void onExportButtonClicked();
    void some_slot();

public:
    QPushButton *newButton{};
    QPushButton *filterButton{};
    QPushButton *exportButton{};

    QRadioButton *hexRadio{};
    QRadioButton *decRadio{};
    QRadioButton *twoBytes{};
    QRadioButton *oneByte{};
    QRadioButton *eqRadio{};
    QRadioButton *all{};
    QRadioButton *leRadio{};
    QRadioButton *leeqRadio{};
    QRadioButton *grRadio{};
    QRadioButton *greqRadio{};
    QRadioButton *increased{};
    QRadioButton *decreased{};
    QRadioButton *increased_or_save{};
    QRadioButton *decreased_or_save{};
    QRadioButton *save{};
    QRadioButton *changed{};
    QLineEdit *compareWith{};

    QCheckBox *checkRom{};
    QCheckBox *checkRam{};

    QTableWidget *tableWidget{};

    Nes *nes;
    std::vector<ResultRaw> result;
};

}  // namespace NES::Cheating

