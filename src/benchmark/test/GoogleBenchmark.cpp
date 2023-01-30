
#include "secret/client/ClientManager.h"
#include "public/client/ClientManager.h"
#include "Timer.h"
#include "SecretBaseApplication.h"

#include <benchmark/benchmark.h>

static void SecretBookOnPlatform(benchmark::State &state) {

    try {
        yakbas::sec::SealKeys sealKeys;
        yakbas::sec::ClientManager clientManager(sealKeys);
        std::cout << "iterations: " << state.counters.size() << std::endl;

        const auto journeyVecPtr = clientManager.Search("Leipzig",
                                                        "Halle");
        size_t index{};
        for (auto _: state) {
            auto bookingResult = clientManager.BookSecretlyOnPlatformAndDecrypt(*journeyVecPtr->at(++index));
            if (bookingResult->journey_id().empty()) {
                std::cout << "JourneyId is null" << std::endl;
                throw std::runtime_error("JourneyId cannot be null");
            }
        }
    } catch (std::exception &exception) {
        std::cerr << "Exception message: " << exception.what() << std::endl;
    }
}

BENCHMARK(SecretBookOnPlatform);

BENCHMARK_MAIN();