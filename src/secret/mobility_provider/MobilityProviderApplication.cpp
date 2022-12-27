
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
        RunTests(argc, argv);
        this->StartServer();
    }

    void MobilityProviderApplication::StartServer() {
        const auto serverManager = GetUnique<MobilityProviderServerManager>(
                GetShared<MobilityProviderServiceImpl>(),
                SECRET_MOBILITY_PROVIDER_SERVER_PORT,
                "Secret Mobility Provider Server Manager");

        serverManager->Init();
    }

} // yakbas