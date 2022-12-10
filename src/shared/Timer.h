#pragma once

#include <optional>
#include <chrono>
#include <iostream>
#include <memory>

namespace yakbas {

    class Timer {
    public:
        Timer();

        ~Timer();

        void clear();

        void start();

        void stop();

        [[nodiscard]] long long PassedTimeInMillisWithoutStop() const;

        void extraction(std::ostream &os) const;

        long long PassedTimeInMillisWithStop();

        static std::chrono::steady_clock::time_point GetTimePoint();

        static long long GetCurrentTimeMillis();

    private:
        using steady = std::chrono::steady_clock;

        steady::time_point m_begin;
        steady::time_point m_end;
    };

    std::ostream &operator<<(std::ostream &os, const Timer &timer);

}