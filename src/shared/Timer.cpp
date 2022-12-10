
#include "Timer.h"

namespace yakbas {
    using namespace std::literals;

    Timer::Timer() { this->start(); }

    Timer::~Timer() = default;

    void Timer::clear() {
        m_begin = m_end = steady::now();
    }

    void Timer::start() {
        m_begin = steady::now();
    }

    void Timer::stop() {
        m_end = steady::now();
    }

    long long Timer::PassedTimeInMillisWithoutStop() const {
        if (m_end <= m_begin) {
            return 0.0;
        }
        return (m_end - m_begin) / 1ms;
    }

    long long Timer::PassedTimeInMillisWithStop() {
        this->stop();
        return this->PassedTimeInMillisWithoutStop();
    }

    void Timer::extraction(std::ostream &os) const {
        os << PassedTimeInMillisWithoutStop();
    }

    std::chrono::steady_clock::time_point Timer::GetTimePoint() {
        return steady::now();
    }

    long long Timer::GetCurrentTimeMillis() {
        return steady::now().time_since_epoch().count();
    }

    std::ostream &operator<<(std::ostream &os, const Timer &timer) {
        timer.extraction(os);
        return os;
    }
}
