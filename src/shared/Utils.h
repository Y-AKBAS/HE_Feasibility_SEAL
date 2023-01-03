
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
#include "ApplicationConstants.h"


namespace yakbas::util {

    using num_variant = std::variant<std::uint64_t, double, int>;
    extern const std::unique_ptr<log4cplus::Logger> utilLogger;
    extern const std::unique_ptr<std::mt19937> mtPtr;

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

    template<typename... Ts>
    struct LambdaOverloader : Ts ... {
        using Ts::operator()...;

        explicit LambdaOverloader(Ts...ts) {}
    };

    template<typename T>
    T GetRandomNumber() {
        static auto distribution = std::uniform_real_distribution<double>(constants::APP_MIN_RANDOM_NUMBER,
                                                                          constants::APP_MAX_RANDOM_NUMBER);
        return static_cast<T>(distribution(*mtPtr));
    }

    std::shared_ptr<std::stringstream> GetSharedStream();

    std::unique_ptr<std::stringstream> GetUniqueStream();

    std::unique_ptr<std::stringstream> GetUniqueStream(const std::string &message);

    std::string GetUUID();

    bool CompareWithDecimalTolerance(const double *left, const double *right, int precision = 5);

    long ToDec(const std::string &hexVal);

    std::string DoubleToHex(const double *val);

    double HexToDouble(const std::string &val);

    using FilePtr = std::unique_ptr<FILE, decltype([](FILE *f) { fclose(f); })>;

    [[nodiscard]] FilePtr Fopen(const std::filesystem::path &path, std::string_view mode);

    std::ostream &operator<<(std::ostream &os, std::byte b);

}// yakbas