
#include "TransporterApplication.h"
#include "TransporterServerManager.h"
#include "TransporterServiceImpl.h"
#include "Utils.h"

namespace yakbas::pub {
    using namespace yakbas::util;

    TransporterApplication::~TransporterApplication() = default;

    void TransporterApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Public Transporter Application");
            this->StartServer(commandLinePtr.get());
        } catch (std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Public Transporter Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void TransporterApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto serverManager = GetUnique<TransporterServerManager>(
                GetShared<TransporterServiceImpl>(),
                PUBLIC_TRANSPORTER_SERVER_PORT,
                "Public Transporter Server Manager");

        serverManager->Init();
    }

} // yakbas