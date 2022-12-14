
#include "InvoiceClerkApplication.h"
#include "InvoiceClerkServerManager.h"
#include "InvoiceClerkServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    InvoiceClerkApplication::~InvoiceClerkApplication() = default;

    void InvoiceClerkApplication::Run(int argc, char **argv) {
        EnableLogging();
        const auto worker = GetUnique<std::jthread>(&InvoiceClerkApplication::StartServer, this);
        RunTests(argc, argv);
    }

    void InvoiceClerkApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void InvoiceClerkApplication::StartServer() {
        const auto serverManager = GetUnique<InvoiceClerkServerManager>(
                GetShared<InvoiceClerkServiceImpl>(),
                SECRET_INVOICE_CLERK_SERVER_PORT,
                "Secret Invoice Clerk");

        serverManager->Init();
    }

} // yakbas