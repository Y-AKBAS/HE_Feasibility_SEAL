
#include "Utils.h"
#include "ApplicationConstants.h"
#include "Timer.h"

#include <random>
#include <iostream>
#include <log4cplus/loggingmacros.h>

/*
#include <boost/uuid/uuid_generators.hpp> // generators
#include "boost/lexical_cast.hpp"
#include <boost/uuid/uuid_io.hpp>
 */

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

    std::unique_ptr<std::stringstream> GetUniqueStream(const std::string &message) {
        return GetModifiedUnique<std::stringstream>(optionalStreamLambda, message);
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

        return static_cast<std::uint64_t>(distribution(*mtPtr));
    }

    std::string GetUUID() {
        /*
        static const auto uuidGenPtr = GetUnique<boost::uuids::random_generator>();
        return boost::lexical_cast<std::string>((*uuidGenPtr)());
        */

        return "user_" + std::to_string(Timer::GetCurrentTimeNanos());
    }

}