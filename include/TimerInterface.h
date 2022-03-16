#pragma once
#include <chrono>
#include <functional>

namespace NES {

template <typename Functor>
struct TimerInterface {
    explicit TimerInterface(std::chrono::milliseconds duration) {}
    virtual void set_duration(std::chrono::milliseconds) = 0;
    virtual void connect(const Functor &functor) = 0;
    virtual void start() = 0;
};

}