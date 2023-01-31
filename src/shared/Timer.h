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

        [[nodiscard]] std::uint64_t PassedTimeInMillisWithoutStop() const;

        void extraction(std::ostream &os) const;

        std::uint64_t PassedTimeInMillisWithStop();

        static std::chrono::steady_clock::time_point GetSteadyTimePoint();

        static std::chrono::system_clock::time_point GetSystemTimePoint();

        static std::uint64_t GetCurrentTimeNanos();

        static std::uint64_t GetCurrentTimeMillis();

        static std::uint64_t GetCurrentTimeMinutes();

        static std::unique_ptr<google::protobuf::Timestamp> GetTimestamp();

    private:
        using steady = std::chrono::steady_clock;
        using system = std::chrono::system_clock;
        using proto_util = google::protobuf::util::TimeUtil;

        static const std::uint64_t APP_EPOCH;

        steady::time_point m_begin;
        steady::time_point m_end;
    };

    std::ostream &operator<<(std::ostream &os, const Timer &timer);

}