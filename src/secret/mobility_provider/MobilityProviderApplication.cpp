
#include "MobilityProviderApplication.h"
#include "MobilityProviderServerManager.h"
#include "MobilityProviderServiceImpl.h"
#include "Utils.h"
#include "SecretCommandLineInfo.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    MobilityProviderApplication::~MobilityProviderApplication() = default;

    void MobilityProviderApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Secret Mobility Provider Application");
            this->StartServer(commandLinePtr.get());
        } catch (const std::exception &e) {
            const auto &logger = log4cplus::Logger::getInstance("Secret Mobility Provider Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void MobilityProviderApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto &secretCmdLineInfoPtr = reinterpret_cast<SecretCommandLineInfo *>(commandLineInfoPtr);

        if (secretCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const std::string portUrl = secretCmdLineInfoPtr->m_portUrl.empty() ?
                                    SECRET_MOBILITY_PROVIDER_SERVER_PORT : secretCmdLineInfoPtr->m_portUrl;

        const auto serverManager = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>(secretCmdLineInfoPtr->m_sealKeys),
                portUrl,
                "Secret Mobility Provider Server Manager");

        serverManager->Init();
    }

} // yakbas