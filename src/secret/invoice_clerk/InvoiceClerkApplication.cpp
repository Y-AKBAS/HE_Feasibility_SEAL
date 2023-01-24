
#include "InvoiceClerkApplication.h"
#include "InvoiceClerkServerManager.h"
#include "InvoiceClerkServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    InvoiceClerkApplication::~InvoiceClerkApplication() = default;

    void InvoiceClerkApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            auto commandLinePtr = HandleCommandLine(argc, argv, "Secret Invoice Clerk Application");
            CustomSealOperations::GetOperations(commandLinePtr->m_sealKeys);
            RunTests(argc, argv);
            this->StartServer(commandLinePtr.get());
        } catch (const std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Secret Invoice Clerk Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void InvoiceClerkApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto &secretCmdLineInfoPtr = reinterpret_cast<SecretCommandLineInfo *>(commandLineInfoPtr);

        if (secretCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }

        const std::string portUrl = secretCmdLineInfoPtr->m_portUrl.empty() ?
                                    SECRET_INVOICE_CLERK_SERVER_PORT : secretCmdLineInfoPtr->m_portUrl;

        const auto serverManager = GetUnique<InvoiceClerkServerManager>(
                GetShared<InvoiceClerkServiceImpl>(secretCmdLineInfoPtr->m_sealKeys),
                portUrl,
                "Secret Invoice Clerk Server Manager");

        serverManager->Init();
    }

} // yakbas