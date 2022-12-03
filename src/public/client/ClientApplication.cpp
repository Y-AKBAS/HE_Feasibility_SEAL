
#include "ClientApplication.h"
#include "ClientServerManager.h"
#include "ClientServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    ClientApplication::~ClientApplication() = default;

    void ClientApplication::Run() {
        EnableLogging();
        const auto worker = getUnique<std::jthread>(&ClientApplication::StartServer, this);
    }

    void ClientApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void ClientApplication::StartServer() {
        const auto serverManager = getUnique<ClientServerManager>(
                getShared<ClientServiceImpl>(),
                PUBLIC_INVOICE_CLERK_SERVER_PORT,
                "Public Client");

        serverManager->Init();
    }

} // yakbas