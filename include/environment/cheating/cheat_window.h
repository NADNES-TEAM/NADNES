#pragma once

#include <QWidget>
#include <QTabWidget>

namespace NES::Cheating {
class MainWindow;
class CheatWindow : public QWidget {
    Q_OBJECT
public:
    explicit CheatWindow(QWidget *parent = nullptr);
    ~CheatWindow() override = default;
public slots:

public:
    QTabWidget *tabWidget{};
    friend MainWindow;
};

}
