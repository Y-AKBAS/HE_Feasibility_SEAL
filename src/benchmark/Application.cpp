
#include "Application.h"
#include "secret/client/ClientManager.h"
#include "public/client/ClientManager.h"
#include "Timer.h"
#include "SecretBaseApplication.h"

namespace yakbas {

    void Application::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {}

    void Application::Run(int argc, char **argv) {
        try {
            EnableLogging();
            const auto commandLineInfoPtr = sec::SecretBaseApplication::HandleCommandLine(argc, argv,
                                                                                          "Benchmark Commandline");
            Benchmark(*commandLineInfoPtr);

        } catch (const std::exception &exception) {
            const auto logger = log4cplus::Logger::getInstance("Benchmark Error Logger");
            LOG4CPLUS_ERROR(logger, std::string("exception during benchmark. Message: ") + exception.what());
            DisableLogging();
        }
    }

    void Application::Benchmark(const sec::SecretCommandLineInfo &commandLineInfo) {
        sec::ClientManager secretClientManager(commandLineInfo.m_sealKeys);
        pub::ClientManager publicClientManager;
    }


} // yakbas