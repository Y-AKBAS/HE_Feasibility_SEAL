
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
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_begin).count();
    }

    long long Timer::PassedTimeInMillisWithStop() {
        this->stop();
        return this->PassedTimeInMillisWithoutStop();
    }

    std::chrono::steady_clock::time_point Timer::GetSteadyTimePoint() {
        return steady::now();
    }

    std::chrono::system_clock::time_point Timer::GetSystemTimePoint() {
        return system::now();
    }

    long long int Timer::GetCurrentTimeNanos() {
        return system::now().time_since_epoch().count();
    }

    long long int Timer::GetCurrentTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                system::now().time_since_epoch()).count();
    }

    void Timer::extraction(std::ostream &os) const {
        os << PassedTimeInMillisWithoutStop();
    }

    std::unique_ptr<google::protobuf::Timestamp> Timer::GetTimestamp() {
        return std::make_unique<google::protobuf::Timestamp>(proto_util::GetCurrentTime());
    }

    std::ostream &operator<<(std::ostream &os, const Timer &timer) {
        timer.extraction(os);
        return os;
    }
}
