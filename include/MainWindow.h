#pragma once

#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QWidget>
#include <QtGui>
#include <mutex>
#include "KeyboardInterface.h"
#include "ScreenInterface.h"

namespace NES {

class MainWindow : public QMainWindow, public ScreenInterface, public KeyboardInterface {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override = default;

    // Keyboard methods
    KeyboardInterface *get_keyboard_interface();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    [[nodiscard]] uint8_t get_pressed_keys() const override;

    // Screen methods
    ScreenInterface *get_screen_interface();
    void set_pixel(int row, int column, Color color) override;
    void refresh_screen() override;

private:
    QLabel *m_image_label;
    QImage m_screen_image;

    static QMap<Qt::Key, int> m_index_by_key;
    std::atomic<uint8_t> m_pressed_keys_bitset{};
};

}  // namespace NES
