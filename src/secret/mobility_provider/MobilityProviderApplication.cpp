
#include "MobilityProviderApplication.h"
#include "MobilityProviderServerManager.h"
#include "MobilityProviderServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    MobilityProviderApplication::~MobilityProviderApplication() = default;

    void MobilityProviderApplication::Run(int argc, char **argv) {
        EnableLogging();
        const auto worker = GetUnique<std::jthread>(&MobilityProviderApplication::StartServer, this);
        RunTests(argc, argv);
    }

    void MobilityProviderApplication::StartServer() {
        const auto serverManager = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>(),
                SECRET_MOBILITY_PROVIDER_SERVER_PORT,
                "Secret Mobility Provider");

        serverManager->Init();
    }

} // yakbas