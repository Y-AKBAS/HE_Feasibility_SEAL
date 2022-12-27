
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
        RunTests(argc, argv);
        this->StartServer();
    }

    void ClientApplication::StartServer() {
        const auto serverManager = GetUnique<ClientServerManager>(
                GetShared<ClientServiceImpl>(),
                PUBLIC_CLIENT_SERVER_PORT,
                "Public Client Server Manager");

        serverManager->Init();
    }

} // yakbas