
#include "ClientApplication.h"
#include "ClientServerManager.h"
#include "ClientServiceImpl.h"
#include "Utils.h"
#include "SealOperations.h"
#include "UserSealOperations.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    ClientApplication::~ClientApplication() = default;

    void ClientApplication::Run() {
        EnableLogging();
        const auto worker = getUnique<std::jthread>(&ClientApplication::StartServer, this);
        const auto sealOperations = getUnique<UserSealOperations>("Client Logger");
    }

    void ClientApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void ClientApplication::StartServer() {
        const auto serverManager = getUnique<ClientServerManager>(
                getShared<ClientServiceImpl>(),
                SECRET_CLIENT_SERVER_PORT,
                "Secret Client");

        serverManager->Init();
    }

} // yakbas