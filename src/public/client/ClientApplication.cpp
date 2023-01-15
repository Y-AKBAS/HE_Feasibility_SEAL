
#include "ClientApplication.h"
#include "ClientServerManager.h"
#include "ClientServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    ClientApplication::~ClientApplication() = default;

    void ClientApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Public Client Application");
            this->StartServer(commandLinePtr.get());
        } catch (std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Public Client Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void ClientApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto serverManager = GetUnique<ClientServerManager>(
                GetShared<ClientServiceImpl>(),
                PUBLIC_CLIENT_SERVER_PORT,
                "Public Client Server Manager");

        serverManager->Init();
    }

} // yakbas