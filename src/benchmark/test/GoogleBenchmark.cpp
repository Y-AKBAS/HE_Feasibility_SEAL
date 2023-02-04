
#include "secret/client/ClientManager.h"
#include "public/client/ClientManager.h"
#include "secret/secret_shared/SecretCommandLineInfo.h"
#include <benchmark/benchmark.h>
#include <log4cplus/configurator.h>

using namespace yakbas;
using namespace std::string_literals;

const static log4cplus::Logger &benchmarkLogger = log4cplus::Logger::getInstance("Benchmark Logger");
const static log4cplus::Logger &exceptionLogger = log4cplus::Logger::getInstance("Exception Logger");

using bookingFunc = std::function<
        std::unique_ptr<communication::BookingResponse>(
                const communication::Journey &journey
        )>;

const static sec::SecretCommandLineInfo info = []() -> decltype(auto) {
    sec::SecretCommandLineInfo commandLineInfo{};
    commandLineInfo.m_numberOfRequests = 1;
    return commandLineInfo;
}();

static void SecretBookOnPlatform(benchmark::State &state) {
    try {
        sec::ClientManager clientManager(info.m_sealKeys);
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
        sec::ClientManager clientManager(info.m_sealKeys);
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
        sec::ClientManager clientManager(info.m_sealKeys);
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
        sec::ClientManager clientManager(info.m_sealKeys);
        const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
        size_t index{};
        for (auto _: state) {
            auto bookingResult = clientManager.BookSymmetricOnMobilityProvidersAndDecrypt(*journeyVecPtr->at(++index));
            if (bookingResult->journey_id().empty()) {
                LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                throw std::runtime_error("JourneyId cannot be empty");
            }
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

BENCHMARK(SecretBookOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(benchmark::kSecond);
//BENCHMARK(SecretBookOnMobilityProviders)->Iterations(info.m_numberOfRequests)->Unit(benchmark::kSecond);
//BENCHMARK(SecretBookOnPlatformSymmetric)->Iterations(info.m_numberOfRequests)->Unit(benchmark::kSecond);
//BENCHMARK(SecretBookOnMobilityProvidersSymmetric)->Iterations(info.m_numberOfRequests)->Unit(benchmark::kSecond);
BENCHMARK(SecretUsageTest)->Iterations(info.m_numberOfRequests)->Unit(benchmark::kSecond);

int main(int argc, char **argv) {
    char arg0_default[] = "benchmark";
    char *args_default = arg0_default;
    if (!argv) {
        argc = 1;
        argv = &args_default;
    }

    std::cout << BENCHMARK_OUT_FORMAT << std::endl;
    std::cout << BENCHMARK_OUT << std::endl;
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    log4cplus::deinitialize();
    return 0;
}
