#pragma once

#include <QWidget>
#include <QComboBox>

class PlayerSelectWindow: public QWidget {
    Q_OBJECT

public:
    PlayerSelectWindow();
    ~PlayerSelectWindow() override = default;

    void add_player(int index, const QString &name);
    void remove_player(int index);

signals:
    void selected(int, int);

private slots:
    void on_ok_btn_clicked();
    void on_cancel_btn_clicked();

private:
    int m_player1_index{};
    int m_player2_index{};
    QComboBox* m_player1_select;
    QComboBox* m_player2_select;
};