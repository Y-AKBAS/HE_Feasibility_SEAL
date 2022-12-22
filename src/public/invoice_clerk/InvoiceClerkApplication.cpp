
#include "InvoiceClerkApplication.h"
#include "InvoiceClerkServerManager.h"
#include "InvoiceClerkServiceImpl.h"
#include "Utils.h"

#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    InvoiceClerkApplication::~InvoiceClerkApplication() = default;

    void InvoiceClerkApplication::Run(int argc, char **argv) {
        EnableLogging();
        const auto worker = GetUnique<std::jthread>(&InvoiceClerkApplication::StartServer, this);
        RunTests(argc, argv);
    }

    void InvoiceClerkApplication::StartServer() {
        const auto serverManager = GetUnique<InvoiceClerkServerManager>(
                GetShared<InvoiceClerkServiceImpl>(),
                PUBLIC_INVOICE_CLERK_SERVER_PORT,
                "Public Invoice Clerk");

        serverManager->Init();
    }

} // yakbas