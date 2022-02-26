#pragma once
#include "Apu.h"
#include <cstdint>
#include <functional>

namespace NADNES::APU {

class Divider {
public:
    explicit Divider(const std::function<void()> &action, uint16_t reload);

    void reload(uint16_t value);
    void reset();
    void clock();

private:
    uint16_t m_reload{0};
    uint16_t m_counter{0};
    const std::function<void()> &m_action;
};

template <typename T>
class Sequencer {
    // TODO
};

class LengthCounter {
public:
    explicit LengthCounter(uint8_t counter);

    void setCounter(uint8_t counter);

    bool clock(bool disable = false);

private:
    uint8_t m_counter;
};

// Can generate
class Sweep {
public:
private:
    bool m_start{false};
    bool m_loop{false};
    uint16_t m_volume{0};

    Divider m_divider;
};

}  // namespace NADNES::APU
