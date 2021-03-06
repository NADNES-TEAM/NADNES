#pragma once

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QSignalMapper>
#include <QTableWidget>
#include <QTimer>
#include <QWidget>
#include "cheat_db_handler_files.h"
#include "nes_core/nes.h"
#include "search.h"

namespace NES::Cheating {

class SearchCheat : public QWidget {
    Q_OBJECT
public:
    explicit SearchCheat(QWidget *parent, NES::Nes *nes, CheatDbHandler *db_handler);
    ~SearchCheat() override = default;

    void init();
    [[nodiscard]] ParamsOfSearch getParams() const;
    void fillTable();

public slots:
    void onNewButtonClicked();
    void onFilterButtonClicked();
    void onExportButtonClicked();
    void onOkButtonClicked();
    void closeDialog() const;
    void onCellChanged(int row, int column);
    void handleButton(int id);
    void fillPartOfTable();
    void debugSlot();

private:
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
    QLineEdit *defaultSaveEdit{};
    std::string file_name;
    QDialogButtonBox *buttonBox{};
    QWidget *save_default{};

    QCheckBox *checkRom{};
    QCheckBox *checkRam{};

    QTableWidget *tableWidget{};
    QSignalMapper qSignalMapper;

    Nes *nes;
    std::vector<ResultRaw> result;
    CheatDbHandler *cheatDbHandler{};
    int result_printed;
};

}  // namespace NES::Cheating
