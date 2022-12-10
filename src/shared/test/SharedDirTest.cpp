

#ifndef DISABLE_TESTS
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Timer.h"
#include "Utils.h"
#include "ApplicationConstants.h"

#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::test {
    using namespace yakbas::util;

    TEST_SUITE("Shared Lib Tests Suite") {

        TEST_CASE("Shared Lib Tests") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            SUBCASE("Ptr tests") {
                CHECK(GetUnique<Timer>() != nullptr);
                CHECK(GetShared<Timer>() != nullptr);
                CHECK(GetUniqueStream() != nullptr);
                CHECK(GetSharedStream() != nullptr);

                std::string myString = "myString";
                const auto optionalFunction = std::make_optional(
                        [&myString](std::string &str) { str = myString; });

                const auto modifiedUnique = GetModifiedUnique<std::string>(optionalFunction);
                CHECK(*modifiedUnique == myString);

                const auto modifiedShared = GetModifiedShared<std::string>(optionalFunction);
                CHECK(*modifiedShared == myString);
            }

            SUBCASE("Random number range tests") {
                for (int i = 0; i < 50; ++i) {
                    const auto number = GetRandomNumber();
                    CHECK(number >= constants::APP_MIN_RANDOM_NUMBER);
                    CHECK(number <= constants::APP_MAX_RANDOM_NUMBER);
                }
            }

            SUBCASE("Timer PassedTimeInMillisWithStop Test") {

                const auto timer = GetUnique<Timer>();
                const long long millis = 2500;

                std::this_thread::sleep_for(std::chrono::milliseconds(millis));

                CHECK(timer->PassedTimeInMillisWithStop() >= millis);
            }

            SUBCASE("Timer PassedTimeInMillisWithoutStop Test") {

                const auto timer = GetUnique<Timer>();
                const long long millis = 2500;

                LOG4CPLUS_DEBUG(*logger, "Before sleep: " + std::to_string(Timer::GetCurrentTimeMillis()));
                std::this_thread::sleep_for(std::chrono::milliseconds(millis));
                LOG4CPLUS_DEBUG(*logger, "After sleep: " + std::to_string(Timer::GetCurrentTimeMillis()));

                timer->stop();
                CHECK(timer->PassedTimeInMillisWithoutStop() >= millis);
            }

            ::log4cplus::deinitialize();
        }
    }
}

#endif
#endif
