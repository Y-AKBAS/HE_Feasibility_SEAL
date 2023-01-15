
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
            auto commandLinePtr = HandleCommandLine(argc, argv, "Secret Platform Application");
            this->StartServer(commandLinePtr.get());
        } catch (const std::exception &e) {
            const auto &logger = log4cplus::Logger::getInstance("Secret Platform Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void PlatformApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto &secretCmdLineInfoPtr = reinterpret_cast<SecretCommandLineInfo *>(commandLineInfoPtr);

        if (secretCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const auto serverManager = GetUnique<PlatformServerManager>(
                GetShared<PlatformServiceImpl>(secretCmdLineInfoPtr->m_sealKeys),
                SECRET_PLATFORM_SERVER_PORT,
                "Secret Platform Server Manager");

        serverManager->Init();
    }

} // yakbas