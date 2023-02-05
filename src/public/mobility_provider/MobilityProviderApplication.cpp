
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
                                    PUBLIC_MOBILITY_PROVIDER_SERVER_PORT_1 : publicCmdLineInfoPtr->m_portUrl;

        std::unique_ptr<MobilityProviderServerManager> serverManager_1 = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>("Public Mobility Provider Service Impl 1"),
                portUrl,
                "Public Mobility Provider Server Manager 1");

        std::unique_ptr<MobilityProviderServerManager> serverManager_2 = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>("Public Mobility Provider Service Impl 2"),
                PUBLIC_MOBILITY_PROVIDER_SERVER_PORT_2,
                "Public Mobility Provider Server Manager 2");

        m_serverManagersPtr.push_back(std::move(serverManager_1));
        m_serverManagersPtr.push_back(std::move(serverManager_2));

        std::vector<std::jthread> threadVec{m_serverManagersPtr.size()};

        const auto &logger = log4cplus::Logger::getInstance("Public Mobility Provider Application Logger");
        for (const auto &ptr: m_serverManagersPtr) {
            threadVec.emplace_back(&MobilityProviderServerManager::Init, ptr.get());
        }

        LOG4CPLUS_INFO(logger, "Public Mobility Provider Servers will run...");
        for (auto &thread: threadVec) {
            thread.join();
        }
    }

} // yakbas