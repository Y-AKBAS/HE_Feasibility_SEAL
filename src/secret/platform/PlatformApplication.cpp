
#include "PlatformApplication.h"
#include "PlatformServerManager.h"
#include "PlatformServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    PlatformApplication::~PlatformApplication() = default;

    void PlatformApplication::Run() {
        EnableLogging();
        const auto worker = getUnique<std::jthread>(&PlatformApplication::StartServer, this);
    }

    void PlatformApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void PlatformApplication::StartServer() {
        const auto serverManager = getUnique<PlatformServerManager>(
                getShared<PlatformServiceImpl>(),
                SECRET_PLATFORM_SERVER_PORT,
                "Secret Platform");

        serverManager->Init();
    }

} // yakbas