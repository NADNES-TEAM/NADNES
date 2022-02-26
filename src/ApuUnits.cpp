#include "ApuUnits.h"

namespace NADNES::APU {

void Divider::reload(uint16_t value) {
    m_reload = value;
}

void Divider::reset() {
    m_counter = m_reload;
}

void Divider::clock() {
    if (m_counter && (--m_counter) == 0) {
        reset();
        m_action();
    }
}
Divider::Divider(const std::function<void()> &action, uint16_t reload)
    : m_action(action), m_reload(reload), m_counter(reload) {
}

LengthCounter::LengthCounter(uint8_t counter) : m_counter(counter) {
}

void LengthCounter::setCounter(uint8_t counter) {
    m_counter = counter;
}

bool LengthCounter::clock(bool disable) {
    if (disable) {
        m_counter = 0;
    } else if (m_counter > 0) {
        --m_counter;
    }
    return static_cast<bool>(m_counter);
}



}  // namespace NADNES::APU
