
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


namespace yakbas::util {

    extern const std::unique_ptr<log4cplus::Logger> utilLogger;

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

    std::shared_ptr<std::stringstream> GetSharedStream();

    std::unique_ptr<std::stringstream> GetUniqueStream();
    std::unique_ptr<std::stringstream> GetUniqueStream(const std::string &message);

    std::ostream &operator<<(std::ostream &os, std::byte b);

    std::uint64_t GetRandomNumber();

    std::string GetUUID();

}// yakbas