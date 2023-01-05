
#include "PlatformApplication.h"
#include "PlatformServerManager.h"
#include "PlatformServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    PlatformApplication::~PlatformApplication() = default;

    void PlatformApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            this->StartServer();
        } catch (const std::exception &exception) {
            const static auto logger = log4cplus::Logger::getInstance("Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + exception.what());
            DisableLogging();
            throw exception;
        }
    }

    void PlatformApplication::StartServer() {
        const auto serverManager = GetUnique<PlatformServerManager>(
                GetShared<PlatformServiceImpl>(),
                SECRET_PLATFORM_SERVER_PORT,
                "Secret Platform Server Manager");

        serverManager->Init();
    }

} // yakbas