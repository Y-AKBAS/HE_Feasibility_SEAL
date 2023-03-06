
#include "BenchmarkFunctions.h"
#include "secret/client/ClientManager.h"
#include "public/client/ClientManager.h"

namespace yakbas {
    using namespace std::string_literals;

    using bookingFunc = std::function<
            std::unique_ptr<communication::BookingResponse>(
                    const communication::Journey &journey
            )>;

    const static log4cplus::Logger &benchmarkLogger = log4cplus::Logger::getInstance("Benchmark Logger");
    const static log4cplus::Logger &exceptionLogger = log4cplus::Logger::getInstance("Benchmark Exception Logger");

    static void BfvSealContext(benchmark::State &state) {
        for (auto _: state) {
            seal::EncryptionParameters encryptionParameters(seal::scheme_type::bfv);
            encryptionParameters.set_coeff_modulus(seal::CoeffModulus::BFVDefault(SEAL_POLY_MODULUS_DEGREE));
            encryptionParameters.set_poly_modulus_degree(SEAL_POLY_MODULUS_DEGREE);
            encryptionParameters.set_plain_modulus(seal::PlainModulus::Batching(SEAL_POLY_MODULUS_DEGREE, 20));
            benchmark::DoNotOptimize(seal::SEALContext(encryptionParameters));
        }
    }

    static void CkksSealContext(benchmark::State &state) {
        for (auto _: state) {
            seal::EncryptionParameters encryptionParameters(seal::scheme_type::ckks);
            encryptionParameters.set_coeff_modulus(
                    seal::CoeffModulus::Create(SEAL_POLY_MODULUS_DEGREE, constants::ENCODING_BIT_SIZES));
            encryptionParameters.set_poly_modulus_degree(SEAL_POLY_MODULUS_DEGREE);
            benchmark::DoNotOptimize(seal::SEALContext(encryptionParameters));
        }
    }

    static void BgvSealContext(benchmark::State &state) {
        for (auto _: state) {
            seal::EncryptionParameters encryptionParameters(seal::scheme_type::bgv);
            encryptionParameters.set_coeff_modulus(seal::CoeffModulus::BFVDefault(SEAL_POLY_MODULUS_DEGREE));
            encryptionParameters.set_poly_modulus_degree(SEAL_POLY_MODULUS_DEGREE);
            encryptionParameters.set_plain_modulus(seal::PlainModulus::Batching(SEAL_POLY_MODULUS_DEGREE, 20));
            benchmark::DoNotOptimize(seal::SEALContext(encryptionParameters));
        }
    }

    static void SecretBookAsymmetricOnPlatform(benchmark::State &state) {
        try {
            const std::string from = "Leipzig";
            const std::string to = "Halle";
            sec::ClientManager clientManager;
            for (auto _: state) {
                auto bookingResult = clientManager.BookAsymmetricOnPlatformAndDecrypt(from, to);
                if (bookingResult->journey_id().empty()) {
                    LOG4CPLUS_ERROR(benchmarkLogger, "JourneyId of the result is empty");
                    throw std::runtime_error("JourneyId cannot be empty");
                }
            }
        } catch (std::exception &exception) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception message: "s + exception.what());
        }
    }

    static void SecretBookOnPlatform(benchmark::State &state) {
        try {
            sec::ClientManager clientManager;
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            const auto vec_size = journeyVecPtr->size();
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnPlatformAndDecrypt(*journeyVecPtr->at(index++ % vec_size));
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
            const auto vec_size = journeyVecPtr->size();
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookSymmetricOnPlatformAndDecrypt(
                        *journeyVecPtr->at(index++ % vec_size));
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
            const auto vec_size = journeyVecPtr->size();
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnMobilityProvidersAndDecrypt(
                        *journeyVecPtr->at(index++ % vec_size));
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
            benchmark::DoNotOptimize(clientManager.Search("Leipzig", "Halle"));
            const auto journeyVecPtr = clientManager.Search("Leipzig", "Halle");
            const auto vec_size = journeyVecPtr->size();
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookSymmetricOnMobilityProvidersAndDecrypt(
                        *journeyVecPtr->at(index++ % vec_size));
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

    static void PublicBookAsymmetricOnPlatform(benchmark::State &state) {
        try {
            const std::string from = "Leipzig";
            const std::string to = "Halle";
            pub::ClientManager clientManager;
            for (auto _: state) {
                auto bookingResult = clientManager.BookAsymmetricOnPlatform(from, to);
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
            const auto vec_size = journeyVecPtr->size();
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnPlatform(*journeyVecPtr->at(index++ % vec_size));
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
            const auto vec_size = journeyVecPtr->size();
            size_t index{};
            for (auto _: state) {
                auto bookingResult = clientManager.BookOnMobilityProviders(*journeyVecPtr->at(index++ % vec_size));
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
            if (info.m_isSecret) {
                if (info.m_isContext) {
                    RegisterContextBenchmark(info);
                } else {
                    RegisterSecretRequestBenchmarks(info);
                }
            } else {
                RegisterPublicRequestBenchmarks(info);
            }
            return true;
        } catch (std::exception &e) {
            LOG4CPLUS_ERROR(exceptionLogger, "Exception thrown during bench registration. Message: "s + e.what());
            return false;
        }
    }

    void RegisterContextBenchmark(const sec::SecretCommandLineInfo &info) {
        LOG4CPLUS_INFO(benchmarkLogger, "Will run context benchmarks...");
        auto timeUnit = static_cast<benchmark::TimeUnit>(info.timeUnit);
        BENCHMARK(BgvSealContext)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(BfvSealContext)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(CkksSealContext)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
    }

    void RegisterSecretRequestBenchmarks(const sec::SecretCommandLineInfo &info) {
        LOG4CPLUS_INFO(benchmarkLogger, "Will run secret request benchmarks...");
        auto timeUnit = static_cast<benchmark::TimeUnit>(info.timeUnit);
        //BENCHMARK(SecretBookOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        //BENCHMARK(SecretBookOnPlatformSymmetric)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        //BENCHMARK(SecretBookOnMobilityProvidersSymmetric)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        //BENCHMARK(SecretSymmetricUsageTest)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(SecretBookAsymmetricOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(SecretBookOnMobilityProviders)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(SecretUsageTest)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
    }

    void RegisterPublicRequestBenchmarks(const sec::SecretCommandLineInfo &info) {
        LOG4CPLUS_INFO(benchmarkLogger, "Will run public request benchmarks...");
        auto timeUnit = static_cast<benchmark::TimeUnit>(info.timeUnit);
        //BENCHMARK(PublicBookOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(PublicBookAsymmetricOnPlatform)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(PublicBookOnMobilityProviders)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
        BENCHMARK(PublicUsageTest)->Iterations(info.m_numberOfRequests)->Unit(timeUnit);
    }
}