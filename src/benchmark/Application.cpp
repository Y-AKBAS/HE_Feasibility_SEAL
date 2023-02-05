
#include "Application.h"
#include "secret/client/ClientManager.h"
#include "public/client/ClientManager.h"
#include "BenchmarkFunctions.h"
#include "Timer.h"
#include "SecretBaseApplication.h"
#include "benchmark/benchmark.h"

namespace yakbas {

    void Application::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        throw std::runtime_error("Benchmark doesn't have a server");
    }

    void Application::Run(int argc, char **argv) {
        try {
            EnableLogging();
            const auto commandLineInfoPtr = sec::SecretBaseApplication::HandleCommandLine(argc, argv,
                                                                                          "Benchmark Commandline");
            sec::CustomSealOperations::GetOperations(commandLineInfoPtr->m_sealKeys);
            Benchmark(*commandLineInfoPtr, argc, argv);
        } catch (const std::exception &exception) {
            const auto logger = log4cplus::Logger::getInstance("Benchmark Error Logger");
            LOG4CPLUS_ERROR(logger, std::string("exception during benchmark. Message: ") + exception.what());
        }
        DisableLogging();
    }

    void Application::Benchmark(const sec::SecretCommandLineInfo &commandLineInfo, int argc, char **argv) {
        benchmark::Initialize(&argc, argv);
        if (RegisterFunctions(commandLineInfo)) {
            benchmark::RunSpecifiedBenchmarks();
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        benchmark::Shutdown();
    }

} // yakbas