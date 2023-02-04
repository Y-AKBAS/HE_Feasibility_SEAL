
#include "BenchmarkFunctions.h"
#include "secret/client/ClientManager.h"
#include "public/client/ClientManager.h"

namespace yakbas {
    using namespace std::string_literals;

    using bookingFunc = std::function<
            std::unique_ptr<communication::BookingResponse>(
                    const communication::Journey &journey
            )>;

    const static sec::SecretCommandLineInfo secretCommandLineInfo = []() -> decltype(auto) {
        sec::SecretCommandLineInfo commandLineInfo{};
        commandLineInfo.m_numberOfRequests = 1;
        return commandLineInfo;
    }();

    const static log4cplus::Logger &benchmarkLogger = log4cplus::Logger::getInstance("Benchmark Logger");
    const static log4cplus::Logger &exceptionLogger = log4cplus::Logger::getInstance("Exception Logger");

    static void SecretBookOnPlatform(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnPlatformAndDecrypt(*journeyVecPtr->at(++index));
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void SecretBookOnPlatformSymmetric(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookSymmetricOnPlatformAndDecrypt(*journeyVecPtr->at(++index));
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void SecretBookOnMobilityProviders(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnMobilityProvidersAndDecrypt(*journeyVecPtr->at(++index));
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void SecretBookOnMobilityProvidersSymmetric(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookSymmetricOnMobilityProvidersAndDecrypt(
                        *journeyVecPtr->at(++index));
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void SecretUsageTest(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            for (auto _: state) {
                clientManager.SendStartUsingRequest();
                clientManager.SendEndUsingRequest();
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void SecretSymmetricUsageTest(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            for (auto _: state) {
                clientManager.SendStartUsingRequestSymmetric();
                clientManager.SendEndUsingRequestSymmetric();
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void PublicBookOnPlatform(benchmark::State &state) {
        try {
            pub::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnPlatform(*journeyVecPtr->at(++index));
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void PublicBookOnMobilityProviders(benchmark::State &state) {
        try {
            pub::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnMobilityProviders(*journeyVecPtr->at(++index));
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void PublicUsageTest(benchmark::State &state) {
        try {
            pub::ClientManager clientManager;
            for (auto _: state) {
                clientManager.SendStartUsingRequest();
                clientManager.SendEndUsingRequest();
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    bool RegisterFunctions(const sec::SecretCommandLineInfo &info) {
        try {
            auto timeUnit = static_cast<benchmark::TimeUnit>(info.timeUnit);
            /*BENCHMARK(SecretBookOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            BENCHMARK(SecretBookOnPlatformSymmetric)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            BENCHMARK(SecretBookOnMobilityProviders)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            BENCHMARK(SecretBookOnMobilityProvidersSymmetric)->Iterations(info.m_numberOfRequests)->Unit(
                    timeUnit);
            BENCHMARK(SecretUsageTest)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            BENCHMARK(SecretSymmetricUsageTest)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);*/
            BENCHMARK(PublicBookOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            BENCHMARK(PublicBookOnMobilityProviders)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            BENCHMARK(PublicUsageTest)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
            return true;
        } catch (std::exception &e) {
            return false;
        }
    }
}