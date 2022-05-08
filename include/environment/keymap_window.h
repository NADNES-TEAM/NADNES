#pragma once

#include <QMap>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include "environment/main_window.h"

namespace NES {

class KeymapWindow : public QWidget {
    Q_OBJECT

public:
    KeymapWindow(QMap<Qt::Key, NES::Keys> &key_to_btn);
    ~KeymapWindow() override = default;

private slots:
    void on_table_cellChanged(int row, int col);
    void on_restore_btn_clicked();
    void on_cancel_btn_clicked();
    void on_apply_btn_clicked();
    void on_save_btn_clicked();

private:
    QTableWidget *table;
    QMap<NES::Keys, Qt::Key> btn_to_key;
    QMap<Qt::Key, NES::Keys> &key_to_btn;
};

}  // namespace NES
