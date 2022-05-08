#pragma once

#include <QKeySequenceEdit>
#include <QMap>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include "environment/main_window.h"

namespace NES {

class KeymapWindow : public QWidget {
    Q_OBJECT

public:
    explicit KeymapWindow(QMap<Qt::Key, NES::Keys> &key_to_btn);
    ~KeymapWindow() override = default;

private slots:
    void on_restore_btn_clicked();
    void on_cancel_btn_clicked();
    void on_apply_btn_clicked();
    void on_save_btn_clicked();
    void on_sequence_edit_editingFinished();
    void on_table_widget_itemDoubleClicked();

private:
    QTableWidget *m_table;
    QMap<NES::Keys, Qt::Key> btn_to_key;
    QMap<Qt::Key, NES::Keys> &key_to_btn;
    QKeySequenceEdit *m_sequence;
};

}  // namespace NES
