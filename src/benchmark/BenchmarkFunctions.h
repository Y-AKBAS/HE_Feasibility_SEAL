#pragma once

#include "SecretCommandLineInfo.h"
#include <benchmark/benchmark.h>

namespace yakbas {

    static void BfvSealContext(benchmark::State &state);

    static void CkksSealContext(benchmark::State &state);

    static void BgvSealContext(benchmark::State &state);

    static void SecretBookAsymmetricOnPlatform(benchmark::State &state);

    static void SecretBookOnPlatform(benchmark::State &state);

    static void SecretBookOnPlatformSymmetric(benchmark::State &state);

    static void SecretBookOnMobilityProviders(benchmark::State &state);

    static void SecretBookOnMobilityProvidersSymmetric(benchmark::State &state);

    static void SecretUsageTest(benchmark::State &state);

    static void SecretSymmetricUsageTest(benchmark::State &state);

    static void PublicBookAsymmetricOnPlatform(benchmark::State &state);

    static void PublicBookOnPlatform(benchmark::State &state);

    static void PublicBookOnMobilityProviders(benchmark::State &state);

    static void PublicUsageTest(benchmark::State &state);

    bool RegisterFunctions(const sec::SecretCommandLineInfo &info);

    void RegisterContextBenchmark(const sec::SecretCommandLineInfo &info);

    void RegisterSecretRequestBenchmarks(const sec::SecretCommandLineInfo &info);

    void RegisterPublicRequestBenchmarks(const sec::SecretCommandLineInfo &info);
}
