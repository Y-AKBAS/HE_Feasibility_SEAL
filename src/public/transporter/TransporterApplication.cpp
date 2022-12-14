
#include "TransporterApplication.h"
#include "TransporterServerManager.h"
#include "TransporterServiceImpl.h"
#include "Utils.h"

#include <iostream>
#include <log4cplus/configurator.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    TransporterApplication::~TransporterApplication() = default;

    void TransporterApplication::Run(int argc, char **argv) {
        EnableLogging();
        const auto worker = GetUnique<std::jthread>(&TransporterApplication::StartServer, this);
        RunTests(argc, argv);
    }

    void TransporterApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void TransporterApplication::StartServer() {
        const auto serverManager = GetUnique<TransporterServerManager>(
                GetShared<TransporterServiceImpl>(),
                PUBLIC_TRANSPORTER_SERVER_PORT,
                "Public Transporter");

        serverManager->Init();
    }

} // yakbas