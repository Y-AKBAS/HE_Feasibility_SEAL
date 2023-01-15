
#include "TransporterApplication.h"
#include "TransporterServerManager.h"
#include "TransporterServiceImpl.h"
#include "Utils.h"

#include <iostream>
#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    TransporterApplication::~TransporterApplication() = default;

    void TransporterApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Secret Transporter Application");
            this->StartServer(commandLinePtr.get());
        } catch (const std::exception &e) {
            const auto &logger = log4cplus::Logger::getInstance("Secret Transporter Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }

    }

    void TransporterApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto &secretCmdLineInfoPtr = reinterpret_cast<SecretCommandLineInfo *>(commandLineInfoPtr);

        if (secretCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const std::string portUrl = secretCmdLineInfoPtr->m_portUrl.empty() ?
                                    SECRET_TRANSPORTER_SERVER_PORT : secretCmdLineInfoPtr->m_portUrl;

        const auto serverManager = GetUnique<TransporterServerManager>(
                GetShared<TransporterServiceImpl>(secretCmdLineInfoPtr->m_sealKeys),
                portUrl,
                "Secret Transporter Server Manager");

        serverManager->Init();
    }

} // yakbas