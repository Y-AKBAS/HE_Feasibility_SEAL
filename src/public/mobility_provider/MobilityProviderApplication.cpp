
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
        EnableLogging();
        const auto worker = GetUnique<std::jthread>(&MobilityProviderApplication::StartServer, this);
    }

    void MobilityProviderApplication::StartServer() {
        const auto serverManager = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>(),
                PUBLIC_MOBILITY_PROVIDER_SERVER_PORT,
                "Public Mobility Provider");

        serverManager->Init();
    }

} // yakbas