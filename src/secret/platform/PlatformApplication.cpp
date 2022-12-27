
#include "PlatformApplication.h"
#include "PlatformServerManager.h"
#include "PlatformServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    PlatformApplication::~PlatformApplication() = default;

    void PlatformApplication::Run(int argc, char **argv) {
        EnableLogging();
        RunTests(argc, argv);
        this->StartServer();
    }

    void PlatformApplication::StartServer() {
        const auto serverManager = GetUnique<PlatformServerManager>(
                GetShared<PlatformServiceImpl>(),
                SECRET_PLATFORM_SERVER_PORT,
                "Secret Platform Server Manager");

        serverManager->Init();
    }

} // yakbas