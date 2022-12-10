
#include "Utils.h"
#include "ApplicationConstants.h"
#include <random>
#include <iostream>
#include <log4cplus/loggingmacros.h>

namespace yakbas::util {

    extern const std::unique_ptr<log4cplus::Logger> utilLogger
            = GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("UtilLogger"));

    const auto optionalStreamLambda = std::make_optional([](std::stringstream &stream) {
        stream.exceptions(std::ios::badbit | std::ios::failbit);
    });

    std::shared_ptr<std::stringstream> GetSharedStream() {
        return GetModifiedShared<std::stringstream>(optionalStreamLambda);
    }

    std::unique_ptr<std::stringstream> GetUniqueStream() {
        return GetModifiedUnique<std::stringstream>(optionalStreamLambda);
    }

    std::ostream &operator<<(std::ostream &os, std::byte b) {
        return os << std::bitset<8>(std::to_integer<int>(b));
    }

    std::uint64_t GetRandomNumber() {
        static const auto mtPtr = []() -> decltype(auto) {
            LOG4CPLUS_DEBUG(*utilLogger, "Random generator engine being created...");
            std::random_device device;
            std::seed_seq seedSeq{device()};
            return GetUnique<std::mt19937>(seedSeq);
        }();
        static auto distribution = std::uniform_real_distribution<double>(constants::APP_MIN_RANDOM_NUMBER,
                                                                          constants::APP_MAX_RANDOM_NUMBER);
        distribution.reset();
        return static_cast<std::uint64_t>(distribution(*mtPtr));
    }

}