
#include "InvoiceClerkApplication.h"
#include "InvoiceClerkServerManager.h"
#include "InvoiceClerkServiceImpl.h"

#include <iostream>
#include <log4cplus/configurator.h>

namespace yakbas::pub {

        InvoiceClerkApplication::~InvoiceClerkApplication() = default;

        void InvoiceClerkApplication::Run() {
            EnableLogging();
            const auto worker = std::make_unique<std::jthread>(&InvoiceClerkApplication::StartServer, this);
        }

        void InvoiceClerkApplication::EnableLogging() {
            log4cplus::initialize();
            log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
        }

        void InvoiceClerkApplication::StartServer() {
            const auto serverManager = std::make_unique<InvoiceClerkServerManager>(
                    std::make_shared<InvoiceClerkServiceImpl>(),
                    PUBLIC_INVOICE_CLERK_SERVER_PORT,
                    "Public Invoice Clerk");

            serverManager->Init();
        }

    } // pub