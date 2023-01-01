
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
            RunTests(argc, argv);
            this->StartServer();
        } catch (const std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }

    }

    void InvoiceClerkApplication::StartServer() {
        const auto serverManager = GetUnique<InvoiceClerkServerManager>(
                GetShared<InvoiceClerkServiceImpl>(),
                SECRET_INVOICE_CLERK_SERVER_PORT,
                "Secret Invoice Clerk Server Manager");

        serverManager->Init();
    }

} // yakbas