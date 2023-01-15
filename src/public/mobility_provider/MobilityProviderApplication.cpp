
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
        const auto &publicCmdLineInfoPtr = reinterpret_cast<PublicCommandLineInfo *>(commandLineInfoPtr);

        if (publicCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const std::string portUrl = publicCmdLineInfoPtr->m_portUrl.empty() ?
                                    PUBLIC_MOBILITY_PROVIDER_SERVER_PORT : publicCmdLineInfoPtr->m_portUrl;

        const auto serverManager = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>(),
                portUrl,
                "Public Mobility Provider Server Manager");

        serverManager->Init();
    }

} // yakbas