#pragma once

#include <QKeySequenceEdit>
#include <QMap>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include "default_keymaps.h"

class KeymapWindow : public QWidget {
    Q_OBJECT

public:
    explicit KeymapWindow(QMap<Qt::Key, NES::Keys> &key_to_btn,
                          QMap<NES::Keys, Qt::Key> &btn_to_key);
    ~KeymapWindow() override = default;
    void update_table();

    NES::Players m_player = NES::Players::None;

private slots:
    void on_restore_btn_clicked();
    void on_cancel_btn_clicked();
    void on_apply_btn_clicked();
    void on_save_btn_clicked();
    void on_sequence_edit_editingFinished();
    void on_table_widget_itemDoubleClicked();

private:
    QTableWidget *m_table;
    QMap<Qt::Key, NES::Keys> &m_key_to_btn;
    QMap<NES::Keys, Qt::Key> &m_btn_to_key;
    QKeySequenceEdit *m_sequence;
};
