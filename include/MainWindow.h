#pragma once

#include <QLabel>
#include <QWidget>
#include <mutex>
#include <QtGui>
#include <QGraphicsScene>
#include "KeyboardInterface.h"
#include "ScreenInterface.h"

namespace NES {

class QtMainWindow : public QWindow, public ScreenInterface, public KeyboardInterface {
    Q_OBJECT

public:
    explicit QtMainWindow(QWindow *parent = nullptr);

    // Keyboard methods
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    [[nodiscard]] uint8_t getPressedKeys() const override;

    // Screen methods
    void setPixel(int row, int column, Color color) override;

private:
    QGraphicsScene *scene;
    QImage m_screenImage;

    static QMap<Qt::Key, int> m_indexByKey;
    std::atomic<uint8_t> m_pressedKeysBitset{};
};

}  // namespace NES