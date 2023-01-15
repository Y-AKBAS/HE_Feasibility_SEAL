
#include "MobilityProviderApplication.h"
#include "MobilityProviderServerManager.h"
#include "MobilityProviderServiceImpl.h"
#include "Utils.h"

#include <iostream>
#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    MobilityProviderApplication::~MobilityProviderApplication() = default;

    void MobilityProviderApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Public Mobility Provider Application");
            this->StartServer(commandLinePtr.get());
        } catch (std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Public Mobility Provider Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void MobilityProviderApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto serverManager = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>(),
                PUBLIC_MOBILITY_PROVIDER_SERVER_PORT,
                "Public Mobility Provider Server Manager");

        serverManager->Init();
    }

} // yakbas