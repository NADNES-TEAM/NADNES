#pragma once

#include <QLabel>
#include <QWidget>
#include <mutex>
#include <QtGui>
#include <QGraphicsScene>
#include <QMainWindow>
#include "KeyboardInterface.h"
#include "ScreenInterface.h"

namespace NES {

class MainWindow : public QMainWindow, public ScreenInterface, public KeyboardInterface {
    Q_OBJECT

public:
    MainWindow();

    // Keyboard methods
    KeyboardInterface *getKeyboardInterface();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    [[nodiscard]] uint8_t getPressedKeys() const override;

    // Screen methods
    ScreenInterface *getScreenInterface();
    void setPixel(int row, int column, Color color) override;
    void refreshScreen() override;

private:
    QLabel *m_imageLabel;
    QImage m_screenImage;

    static QMap<Qt::Key, int> m_indexByKey;
    std::atomic<uint8_t> m_pressedKeysBitset{};
};

}  // namespace NES