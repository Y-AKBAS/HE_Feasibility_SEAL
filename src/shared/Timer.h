#pragma once

#include <optional>
#include <chrono>
#include <iostream>
#include <memory>
#include <google/protobuf/util/time_util.h>

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

        static std::chrono::steady_clock::time_point GetSteadyTimePoint();

        static std::chrono::system_clock::time_point GetSystemTimePoint();

        static long long int GetCurrentTimeNanos();

        static long long int GetCurrentTimeMillis();

        static std::unique_ptr<google::protobuf::Timestamp> GetTimestamp();

    private:
        using steady = std::chrono::steady_clock;
        using system = std::chrono::system_clock;
        using proto_util = google::protobuf::util::TimeUtil;

        steady::time_point m_begin;
        steady::time_point m_end;
    };

    std::ostream &operator<<(std::ostream &os, const Timer &timer);

}