
#pragma once

#include <string>
#include <memory>
#include <optional>
#include <functional>
#include <bitset>
#include <memory>
#include <ostream>
#include <sstream>
#include <log4cplus/logger.h>
#include <filesystem>
#include <random>
#include <variant>
#include <type_traits>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/wrappers.pb.h>
#include <log4cplus/loggingmacros.h>
#include "ApplicationConstants.h"


namespace yakbas::util {

    using num_variant = std::variant<std::uint64_t, double, int>;
    using namespace std::string_literals;

    extern const std::unique_ptr<std::mt19937> mtPtr;

    template<typename... Ts>
    struct LambdaOverloader : Ts ... {
        using Ts::operator()...;

        LambdaOverloader(Ts...ts) {}
    };

    const LambdaOverloader anyToNumOverloader = {
            [](const std::uint64_t value, const google::protobuf::Any *any) -> decltype(auto) {
                google::protobuf::UInt64Value uInt64Value{};
                any->UnpackTo(&uInt64Value);
                return uInt64Value.value();
            },
            [](const int value, const google::protobuf::Any *any) -> decltype(auto) {
                google::protobuf::Int32Value int32Value{};
                any->UnpackTo(&int32Value);
                return int32Value.value();
            },
            [](const double value, const google::protobuf::Any *any) -> decltype(auto) {
                google::protobuf::DoubleValue doubleValue{};
                any->UnpackTo(&doubleValue);
                return doubleValue.value();
            }
    };

    const LambdaOverloader numToAnyOverloader = {
            [](const std::uint64_t value, google::protobuf::Any *any) {
                google::protobuf::UInt64Value uInt64Value{};
                uInt64Value.set_value(value);
                any->PackFrom(uInt64Value);
            },
            [](const int value, google::protobuf::Any *any) {
                google::protobuf::Int32Value int32Value{};
                int32Value.set_value(value);
                any->PackFrom(int32Value);
            },
            [](const double value, google::protobuf::Any *any) {
                google::protobuf::DoubleValue doubleValue{};
                doubleValue.set_value(value);
                any->PackFrom(doubleValue);
            }
    };

    template<typename T, typename U>
    constexpr bool areSameType = std::is_same_v<std::decay_t<T>, U>;

    template<typename T>
    constexpr bool isSameType(const auto &arg) {
        return std::is_same_v<T, std::decay_t<decltype(arg)>>;
    }

    template<typename T>
    constexpr const auto numCaster = [](const auto &value) -> T { return static_cast<T>(value); };

    template<typename T>
    constexpr std::unique_ptr<T> GetUnique(const auto &... constructorParams) {
        return std::make_unique<T>(constructorParams...);
    }

    template<typename T>
    constexpr std::unique_ptr<T> GetModifiedUnique(const std::optional<std::function<void(T &t)>> &function,
                                                   const auto &... constructorParams) {
        auto ptr = std::make_unique<T>(constructorParams...);
        if (function.has_value()) {
            const auto &func = function.value();
            func(*ptr);
        }
        return ptr;
    }

    template<typename T>
    constexpr std::shared_ptr<T> GetShared(const auto &... constructorParams) {
        return std::make_shared<T>(constructorParams...);
    }

    template<typename T>
    constexpr std::shared_ptr<T> GetModifiedShared(const std::optional<std::function<void(T &t)>> &function,
                                                   const auto &... constructorParams) {
        auto ptr = GetShared<T>(constructorParams...);
        if (function.has_value()) {
            const auto &func = function.value();
            func(*ptr);
        }
        return ptr;
    }

    template<typename T>
    T GetRandomNumber() {
        static auto distribution = std::uniform_real_distribution<double>(constants::APP_MIN_RANDOM_NUMBER,
                                                                          constants::APP_MAX_RANDOM_NUMBER);
        return static_cast<T>(distribution(*mtPtr));
    }

    template<typename T>
    num_variant GetRandomNumberVariant() {
        return GetRandomNumber<T>();
    }

    template<typename T = num_variant>
    T AnyToNum(const google::protobuf::Any *any) {
        const T t{};
        return anyToNumOverloader(t, any);
    }

    template<typename T = num_variant>
    num_variant AnyToNumVariant(const google::protobuf::Any *any) {
        return AnyToNum<T>(any);
    }

    template<typename T = num_variant>
    void NumToAny(google::protobuf::Any *any) {
        const auto num = GetRandomNumber<T>();
        return numToAnyOverloader(num, any);
    }

    template<typename T = num_variant>
    void NumToAny(T num, google::protobuf::Any *any) {
        return numToAnyOverloader(num, any);
    }

    template<typename T = num_variant>
    T GetAnyVariant(const num_variant *variant) {
        return std::visit(numCaster<T>, *variant);
    }

    template<typename T = num_variant>
    bool CompareWithDecimalTolerance(const T *left, const T *right, int precision = 3) {
        constexpr const double ten = 10.0;
        const double scale = std::pow(ten, precision);
        const double leftTrunc = std::trunc(*left * scale) / scale;
        const double rightTrunc = std::trunc(*right * scale) / scale;
        const double tolerance = ten / scale;
        return std::abs(leftTrunc - rightTrunc) < tolerance;
    }

    template<typename T>
    bool CompareWithTolerance(const T *left, const T *right, int tolerance = 1) {
        return std::abs(left - right) < tolerance;
    }

    void NumVariantToAny(const num_variant *variant, google::protobuf::Any *any);

    num_variant AnyToNumVariant(bool isCKKS, const google::protobuf::Any *any);

    double AnyToNum(bool isCKKS, const google::protobuf::Any *any);

    std::shared_ptr<std::stringstream> GetSharedStream();

    std::unique_ptr<std::stringstream> GetUniqueStream();

    std::unique_ptr<std::stringstream> GetUniqueStream(const std::string &message);

    std::string GetUUID();

    long ToDec(const std::string &hexVal);

    std::string DoubleToHex(const double *val);

    double HexToDouble(const std::string &val);

    using FilePtr = std::unique_ptr<FILE, decltype([](FILE *f) { fclose(f); })>;

    [[nodiscard]] FilePtr Fopen(const std::filesystem::path &path, std::string_view mode);

    std::ostream &operator<<(std::ostream &os, std::byte b);

}// yakbas