
#include "ClientApplication.h"
#include "ClientServerManager.h"
#include "ClientServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    ClientApplication::~ClientApplication() = default;

    void ClientApplication::Run(int argc, char **argv) {
        EnableLogging();
        const auto worker = GetUnique<std::jthread>(&ClientApplication::StartServer, this);
        RunTests(argc, argv);
    }

    void ClientApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void ClientApplication::StartServer() {
        const auto serverManager = GetUnique<ClientServerManager>(
                GetShared<ClientServiceImpl>(),
                PUBLIC_CLIENT_SERVER_PORT,
                "Public Client");

        serverManager->Init();
    }

} // yakbas