
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif

#include "doctest/doctest.h"

#if (!DISABLE_TESTS)

#include "Timer.h"
#include "Utils.h"
#include "ApplicationConstants.h"
#include <chrono>

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

            SUBCASE("Timer GetCurrentTimeNanos Test") {
                const long long int nanos = Timer::GetCurrentTimeNanos();
                const std::chrono::duration timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
                const long long int applicationsMillis = Timer::GetCurrentTimeMillis();
                const auto nanosCount = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        timeSinceEpoch).count();
                const auto millisCount = std::chrono::duration_cast<std::chrono::milliseconds>(
                        timeSinceEpoch).count();
                const auto hoursCount = std::chrono::duration_cast<std::chrono::hours>(
                        timeSinceEpoch).count();

                LOG4CPLUS_DEBUG(*logger, "\nnanos: " + std::to_string(nanos) +
                                         "\nnanosCount: " + std::to_string(nanosCount) +
                                         "\nmillisCount: " + std::to_string(millisCount) +
                                         "\napplicationsMillis: " + std::to_string(applicationsMillis) +
                                         "\nhoursCount: " + std::to_string(hoursCount));
                CHECK(nanos < nanosCount);
            }

            SUBCASE("Time measurements") {
                const auto nowResult = std::chrono::system_clock::now().time_since_epoch();

                const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(nowResult).count();
                const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(nowResult).count();
                const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowResult).count();
                const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(nowResult).count();

                const auto systemStream = GetUniqueStream();

                *systemStream << "\nnanoseconds : " << nanoseconds
                              << "\nmicroseconds: " << microseconds
                              << "\nmilliseconds: " << milliseconds
                              << "\nseconds: " << seconds
                              << std::endl;

                LOG4CPLUS_DEBUG(*logger, systemStream->str());

                const auto steadyStream = GetUniqueStream();

                const auto start = std::chrono::steady_clock::now();
                const auto timer = GetUnique<Timer>();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                const auto end = std::chrono::steady_clock::now();
                timer->stop();

                const auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                const auto secondss = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

                const auto timersMillis = timer->PassedTimeInMillisWithoutStop();

                *steadyStream << "\nnanoseconds : " << nanos
                              << "\nmicroseconds: " << micros
                              << "\nmilliseconds: " << millis
                              << "\nseconds: " << secondss
                              << "\ntimer millis: " << timersMillis
                              << std::endl;

                LOG4CPLUS_DEBUG(*logger, steadyStream->str());
                CHECK(timersMillis == millis);
            }

            ::log4cplus::deinitialize();
        }
    }
}

#endif

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#undef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
