
#include "InvoiceClerkApplication.h"
#include "InvoiceClerkServerManager.h"
#include "InvoiceClerkServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    InvoiceClerkApplication::~InvoiceClerkApplication() = default;

    void InvoiceClerkApplication::Run() {
        EnableLogging();
        const auto worker = getUnique<std::jthread>(&InvoiceClerkApplication::StartServer, this);
    }

    void InvoiceClerkApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void InvoiceClerkApplication::StartServer() {
        const auto serverManager = getUnique<InvoiceClerkServerManager>(
                getShared<InvoiceClerkServiceImpl>(),
                SECRET_INVOICE_CLERK_SERVER_PORT,
                "Secret Invoice Clerk");

        serverManager->Init();
    }

} // yakbas