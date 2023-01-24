
#include "MobilityProviderApplication.h"
#include "MobilityProviderServerManager.h"
#include "MobilityProviderServiceImpl.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    MobilityProviderApplication::~MobilityProviderApplication() = default;

    void MobilityProviderApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            auto commandLinePtr = HandleCommandLine(argc, argv, "Secret Mobility Provider Application");
            CustomSealOperations::GetOperations(commandLinePtr->m_sealKeys);
            RunTests(argc, argv);
            this->StartServer(commandLinePtr.get());
        } catch (const std::exception &e) {
            const auto &logger = log4cplus::Logger::getInstance("Secret Mobility Provider Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            for (const auto &ptr: m_serverManagersPtr) {
                ptr->Shutdown();
            }
            DisableLogging();
        }
    }

    void MobilityProviderApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto &secretCmdLineInfoPtr = reinterpret_cast<SecretCommandLineInfo *>(commandLineInfoPtr);

        if (secretCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const std::string portUrl_1 = secretCmdLineInfoPtr->m_portUrl.empty() ?
                                      SECRET_MOBILITY_PROVIDER_SERVER_PORT_1 : secretCmdLineInfoPtr->m_portUrl;

        std::unique_ptr<MobilityProviderServerManager> serverManager_1 = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>("Secret Mobility Provider PlatformImpl 1",
                                                       secretCmdLineInfoPtr->m_sealKeys),
                portUrl_1,
                "Secret Mobility Provider Server Manager 1");

        std::unique_ptr<MobilityProviderServerManager> serverManager_2 = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>("Secret Mobility Provider PlatformImpl 1",
                                                       secretCmdLineInfoPtr->m_sealKeys),
                SECRET_MOBILITY_PROVIDER_SERVER_PORT_2,
                "Secret Mobility Provider Server Manager 2");

        m_serverManagersPtr.push_back(std::move(serverManager_1));
        m_serverManagersPtr.push_back(std::move(serverManager_2));

        std::vector<std::jthread> threadVec{m_serverManagersPtr.size()};

        const auto &logger = log4cplus::Logger::getInstance("Secret Mobility Provider Application Logger");
        for (const auto &ptr: m_serverManagersPtr) {
            threadVec.emplace_back(&MobilityProviderServerManager::Init, ptr.get());
        }

        LOG4CPLUS_INFO(logger, "Mobility Provider Servers will run...");
        for (auto &thread: threadVec) {
            thread.join();
        }
    }
} // yakbas