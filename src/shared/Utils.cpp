
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
            = GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("Util Logger"));

    const auto optionalStreamLambda = std::make_optional([](std::stringstream &stream) {
        stream.exceptions(std::ios::badbit | std::ios::failbit);
    });

    std::shared_ptr<std::stringstream> GetSharedStream() {
        return GetModifiedShared<std::stringstream>(optionalStreamLambda);
    }

    std::unique_ptr<std::stringstream> GetUniqueStream() {
        return GetModifiedUnique<std::stringstream>(optionalStreamLambda,
                                                    std::ios::in | std::ios::out | std::ios::binary);
    }

    std::unique_ptr<std::stringstream> GetUniqueStream(const std::string &message) {
        return GetModifiedUnique<std::stringstream>(optionalStreamLambda, message,
                                                    std::ios::in | std::ios::out | std::ios::binary);
    }

    std::ostream &operator<<(std::ostream &os, std::byte b) {
        return os << std::bitset<8>(std::to_integer<int>(b));
    }

    std::uint64_t GetRandomNumber() {
        static const auto mtPtr = []() -> decltype(auto) {
            LOG4CPLUS_DEBUG(*utilLogger, "Random generator engine is being created...");
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

        return "id_" + std::to_string(Timer::GetCurrentTimeNanos());
    }

    bool CompareWithTolerance(const double *left, const double *right, int precision) {
        constexpr const double ten = 10.0;
        const double scale = std::pow(ten, precision);
        const double leftTrunc = std::trunc(*left * scale) / scale;
        const double rightTrunc = std::trunc(*right * scale) / scale;
        const double tolerance = ten / scale;
        return std::abs(leftTrunc - rightTrunc) < tolerance;
    }

    long toDec(const std::string &hexVal) {
        long num;
        std::stringstream sstream;
        sstream << hexVal;
        sstream >> std::hex >> num;
        return num;
    }

    std::string doubleToHex(const double *val) {
        char buffer[100] = {0};
        snprintf(buffer, 10, "%A", *val);
        return buffer;
    }

    double hexToDouble(const std::string &val) {
        double d = 0.0;
        sscanf_s(val.c_str(), "%lA", &d);
        return d;
    }

}
