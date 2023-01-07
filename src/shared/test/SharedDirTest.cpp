
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif

#include "doctest/doctest.h"

#if (!DISABLE_TESTS)

#include "Timer.h"
#include "Utils.h"
#include "SharedCommunication.pb.h"
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
                    auto number = GetRandomNumber<int>();
                    CHECK(number >= constants::APP_MIN_RANDOM_NUMBER);
                    CHECK(number <= constants::APP_MAX_RANDOM_NUMBER);
                }

                for (int i = 0; i < 50; ++i) {
                    auto number = GetRandomNumber<double>();
                    CHECK(number >= constants::APP_MIN_RANDOM_NUMBER);
                    CHECK(number <= constants::APP_MAX_RANDOM_NUMBER);
                }

                for (int i = 0; i < 50; ++i) {
                    auto number = GetRandomNumber<std::uint64_t>();
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

            SUBCASE("LambdaOverloader Test") {

                const int intValue = 22;
                const float floatValue = 22.5f;
                const double doubleValue = 22.5;

                const LambdaOverloader overloader = {
                        [](int value) -> decltype(auto) { return value; },
                        [](float value) -> decltype(auto) { return value; },
                        [](double value) -> decltype(auto) { return value; }
                };

                CHECK(overloader(intValue) == intValue);
                CHECK(overloader(floatValue) == floatValue);
                CHECK(overloader(doubleValue) == doubleValue);
            }

            SUBCASE("NumToAny and AnyToNum Double Test") {

                const auto beforeNum = GetRandomNumber<double>();
                google::protobuf::Any beforeAny{};
                NumToAny(beforeNum, &beforeAny);

                google::protobuf::Any afterAny{};
                afterAny.CopyFrom(beforeAny);
                const auto afterNum = AnyToNum<double>(&afterAny);
                const auto veryAfterNum = AnyToNum<double>(&afterAny);

                const auto steadyStream = GetUniqueStream();
                *steadyStream << "\nBeforeNum: " << beforeNum << "\nAfterNum: " << afterNum;
                LOG4CPLUS_DEBUG(*logger, steadyStream->str());

                CHECK(beforeNum == afterNum);
                CHECK(veryAfterNum == afterNum);
                CHECK(veryAfterNum == beforeNum);
            }

            SUBCASE("NumToAny and AnyToNum std::uint64_t Test") {

                const auto beforeNum = GetRandomNumber<std::uint64_t>();
                google::protobuf::Any beforeAny{};
                NumToAny(beforeNum, &beforeAny);

                google::protobuf::Any afterAny{};
                afterAny.CopyFrom(beforeAny);
                const auto afterNum = AnyToNum<std::uint64_t>(&afterAny);
                const auto veryAfterNum = AnyToNum<std::uint64_t>(&afterAny);

                google::protobuf::Any lastAny{};
                lastAny.CopyFrom(beforeAny);
                const auto lastNum = static_cast<double>(AnyToNum<std::uint64_t>(&lastAny));

                const auto steadyStream = GetUniqueStream();
                *steadyStream << "\nBeforeNum: " << beforeNum << "\nAfterNum: " << afterNum;
                LOG4CPLUS_DEBUG(*logger, steadyStream->str());

                CHECK(beforeNum == afterNum);
                CHECK(veryAfterNum == afterNum);
                CHECK(veryAfterNum == beforeNum);
                CHECK(lastNum == beforeNum);
            }

            SUBCASE("GetAnyVariant Test") {

                num_variant var = 15.2;
                const auto doubleValue = GetAnyVariant<double>(&var);
                CHECK(isSameType<double>(doubleValue));

                const auto intValue = GetAnyVariant<int>(&var);
                CHECK(isSameType<int>(intValue));

                const auto uintValue = GetAnyVariant<std::uint64_t>(&var);
                CHECK(isSameType<std::uint64_t>(uintValue));
            }

            ::log4cplus::deinitialize();
        }
    }
}

#endif

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#undef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
