
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
        const auto &publicCmdLineInfoPtr = reinterpret_cast<PublicCommandLineInfo *>(commandLineInfoPtr);

        if (publicCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const std::string portUrl = publicCmdLineInfoPtr->m_portUrl.empty() ?
                                    PUBLIC_TRANSPORTER_SERVER_PORT : publicCmdLineInfoPtr->m_portUrl;

        const auto serverManager = GetUnique<TransporterServerManager>(
                GetShared<TransporterServiceImpl>(),
                portUrl,
                "Public Transporter Server Manager");

        serverManager->Init();
    }

} // yakbas