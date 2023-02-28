
#include "Timer.h"

namespace yakbas {
    using namespace std::literals;

    Timer::Timer() { this->start(); }

    Timer::~Timer() = default;

    const std::uint64_t Timer::APP_EPOCH = std::chrono::duration_cast<std::chrono::minutes>(
            system::now().time_since_epoch()).count() - 1;

    void Timer::clear() {
        m_cpu_begin = m_cpu_end = std::clock();
        m_begin = m_end = steady::now();
    }

    void Timer::start() {
        m_cpu_begin = std::clock();
        m_begin = steady::now();
    }

    void Timer::stop() {
        m_cpu_end = std::clock();
        m_end = steady::now();
    }

    std::uint64_t Timer::PassedCpuTimeInMillisWithStop() {
        this->stop();
        return this->PassedCpuTimeInMillisWithoutStop();
    }

    std::uint64_t Timer::PassedTimeInMillisWithStop() {
        this->stop();
        return this->PassedTimeInMillisWithoutStop();
    }

    std::uint64_t Timer::PassedTimeInMillisWithoutStop() const {
        if (m_end <= m_begin) {
            return 0.0;
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_begin).count();
    }

    std::uint64_t Timer::PassedCpuTimeInMillisWithoutStop() const {
        if (m_cpu_end <= m_cpu_begin) {
            return 0.0;
        }
        return static_cast<std::uint64_t>((m_cpu_end - m_cpu_begin) / (CLOCKS_PER_SEC / 1000.0));
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
