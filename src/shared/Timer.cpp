
#include "Timer.h"

namespace yakbas {
    using namespace std::literals;

    Timer::Timer() { this->start(); }

    Timer::~Timer() = default;

    const std::uint64_t Timer::APP_EPOCH = 27919925; // as of 31.01.2023 21:05

    void Timer::clear() {
        m_begin = m_end = steady::now();
    }

    void Timer::start() {
        m_begin = steady::now();
    }

    void Timer::stop() {
        m_end = steady::now();
    }

    std::uint64_t Timer::PassedTimeInMillisWithoutStop() const {
        if (m_end <= m_begin) {
            return 0.0;
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_begin).count();
    }

    std::uint64_t Timer::PassedTimeInMillisWithStop() {
        this->stop();
        return this->PassedTimeInMillisWithoutStop();
    }

    std::chrono::steady_clock::time_point Timer::GetSteadyTimePoint() {
        return steady::now();
    }

    std::chrono::system_clock::time_point Timer::GetSystemTimePoint() {
        return system::now();
    }

    std::uint64_t Timer::GetCurrentTimeNanos() {
        return system::now().time_since_epoch().count();
    }

    std::uint64_t Timer::GetCurrentTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                system::now().time_since_epoch()).count();
    }

    std::uint64_t Timer::GetCurrentTimeMinutes() {
        return std::chrono::duration_cast<std::chrono::minutes>(
                system::now().time_since_epoch()).count() - APP_EPOCH;
    }

    std::unique_ptr<google::protobuf::Timestamp> Timer::GetTimestamp() {
        return std::make_unique<google::protobuf::Timestamp>(proto_util::GetCurrentTime());
    }

    void Timer::extraction(std::ostream &os) const {
        os << PassedTimeInMillisWithoutStop();
    }

    std::ostream &operator<<(std::ostream &os, const Timer &timer) {
        timer.extraction(os);
        return os;
    }
}
