#pragma once

#include <QLabel>
#include <QWidget>
#include <QtGui>

namespace NES {
class KeyBoardProcessor : QWidget {
    Q_OBJECT

public:
    explicit KeyBoardProcessor(QWidget *parent = nullptr);
    [[nodiscard]] uint8_t getSnapshot() const;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    uint8_t m_state{};
    static const std::map<Qt::Key, uint8_t> bitByKey;
};
}  // namespace NES