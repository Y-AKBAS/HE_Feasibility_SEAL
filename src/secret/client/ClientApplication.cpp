
#include "ClientApplication.h"
#include "ClientServerManager.h"
#include "ClientServiceImpl.h"
#include "Utils.h"
#include "SealOperations.h"
#include "CustomSealOperations.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
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
                SECRET_CLIENT_SERVER_PORT,
                "Secret Client Server Manager");

        serverManager->Init();
    }

} // yakbas