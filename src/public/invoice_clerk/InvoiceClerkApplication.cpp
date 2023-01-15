
#include "InvoiceClerkApplication.h"
#include "InvoiceClerkServerManager.h"
#include "InvoiceClerkServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    InvoiceClerkApplication::~InvoiceClerkApplication() = default;

    void InvoiceClerkApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Public Invoice Clerk Application");
            this->StartServer(commandLinePtr.get());
        } catch (std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Public Invoice Clerk Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void InvoiceClerkApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto serverManager = GetUnique<InvoiceClerkServerManager>(
                GetShared<InvoiceClerkServiceImpl>(),
                PUBLIC_INVOICE_CLERK_SERVER_PORT,
                "Public Invoice Clerk Server Manager");

        serverManager->Init();
    }

} // yakbas