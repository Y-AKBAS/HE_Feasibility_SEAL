
#pragma once

#include <string>
#include <memory>
#include <optional>
#include <functional>
#include <bitset>
#include <memory>
#include <ostream>
#include <sstream>


namespace yakbas::util {

    // Add getting random numbers with random number engine and make engine and distribution instances
    // either global or static. Because it is pretty time-consuming to generate them.

    template<typename T>
    constexpr std::unique_ptr<T> getUnique(const auto &... constructorParams) {
        return std::make_unique<T>(constructorParams...);
    }

    template<typename T>
    constexpr std::unique_ptr<T> getModifiedUnique(const std::optional<std::function<void(T &t)>> &function,
                                                   const auto &... constructorParams) {
        auto ptr = std::make_unique<T>(constructorParams...);
        if (function.has_value()) {
            const auto &func = function.value();
            func(*ptr);
        }
        return ptr;
    }

    template<typename T>
    constexpr std::shared_ptr<T> getShared(const auto &... constructorParams) {
        return std::make_shared<T>(constructorParams...);
    }

    template<typename T>
    constexpr std::shared_ptr<T> getModifiedShared(const std::optional<std::function<void(T &t)>> &function,
                                                   const auto &... constructorParams) {
        auto ptr = getShared<T>(constructorParams...);
        if (function.has_value()) {
            const auto &func = function.value();
            func(*ptr);
        }
        return ptr;
    }

    std::shared_ptr<std::stringstream> getSharedStream();

    std::unique_ptr<std::stringstream> getUniqueStream();

    std::ostream &operator<<(std::ostream &os, std::byte b);

}// yakbas