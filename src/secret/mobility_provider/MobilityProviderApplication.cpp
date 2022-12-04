
#include "MobilityProviderApplication.h"
#include "MobilityProviderServerManager.h"
#include "MobilityProviderServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    MobilityProviderApplication::~MobilityProviderApplication() = default;

    void MobilityProviderApplication::Run() {
        EnableLogging();
        const auto worker = getUnique<std::jthread>(&MobilityProviderApplication::StartServer, this);
    }

    void MobilityProviderApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void MobilityProviderApplication::StartServer() {
        const auto serverManager = getUnique<MobilityProviderServerManager>(
                getShared<MobilityProviderServiceImpl>(),
                SECRET_MOBILITY_PROVIDER_SERVER_PORT,
                "Secret Mobility Provider");

        serverManager->Init();
    }

} // yakbas