
#include "Utils.h"
#include "Timer.h"

#include <iostream>

/*
#include <boost/uuid/uuid_generators.hpp> // generators
#include "boost/lexical_cast.hpp"
#include <boost/uuid/uuid_io.hpp>
 */

namespace yakbas::util {

    const std::unique_ptr<std::mt19937> mtPtr = []() -> std::unique_ptr<std::mt19937> {
        std::random_device device;
        std::seed_seq seedSeq{device()};
        return std::make_unique<std::mt19937>(seedSeq);
    }();

    const auto optionalStreamLambda = std::make_optional([](std::stringstream &stream) {
        stream.exceptions(std::ios::badbit | std::ios::failbit);
    });

    num_variant AnyToNumVariant(bool isCKKS, const google::protobuf::Any *any) {
        return isCKKS ? AnyToNumVariant<double>(any) : AnyToNumVariant<std::uint64_t>(any);
    }

    double AnyToNum(bool isCKKS, const google::protobuf::Any *any) {
        return isCKKS ? AnyToNum<double>(any) : static_cast<double>(AnyToNum<std::uint64_t>(any));
    }

    void NumVariantToAny(const num_variant *variant, google::protobuf::Any *any) {

        if (const auto value = std::get_if<std::uint64_t>(variant)) {
            NumToAny(*value, any);
            return;
        }

        if (const auto value = std::get_if<double>(variant)) {
            NumToAny(*value, any);
            return;
        }

        if (const auto value = std::get_if<int>(variant)) {
            NumToAny(*value, any);
            return;
        }

        throw std::invalid_argument("Cannot Convert variant to Any!");
    }

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

    std::string GetUUID() {
        /*
        static const auto uuidGenPtr = GetUnique<boost::uuids::random_generator>();
        return boost::lexical_cast<std::string>((*uuidGenPtr)());
        */

        return "id_" + std::to_string(Timer::GetCurrentTimeNanos());
    }

    long ToDec(const std::string &hexVal) {
        long num;
        std::stringstream sstream;
        sstream << hexVal;
        sstream >> std::hex >> num;
        return num;
    }

    std::string DoubleToHex(const double *val) {
        char buffer[100] = {0};
        snprintf(buffer, 10, "%A", *val);
        return buffer;
    }

    double HexToDouble(const std::string &val) {
        double d = 0.0;
        sscanf_s(val.c_str(), "%lA", &d);
        return d;
    }

}
