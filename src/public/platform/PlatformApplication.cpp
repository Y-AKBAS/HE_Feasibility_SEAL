
#include "PlatformApplication.h"
#include "PlatformServerManager.h"
#include "PlatformServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    PlatformApplication::~PlatformApplication() = default;

    void PlatformApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Public Platform Application");
            this->StartServer(commandLinePtr.get());
        } catch (std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Public Platform Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void PlatformApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto serverManager = GetUnique<PlatformServerManager>(
                GetShared<PlatformServiceImpl>(),
                PUBLIC_PLATFORM_SERVER_PORT,
                "Public Platform Server Manager");

        serverManager->Init();
    }

} // yakbas