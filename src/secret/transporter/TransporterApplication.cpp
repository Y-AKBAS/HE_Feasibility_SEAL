
#include "TransporterApplication.h"
#include "TransporterServerManager.h"
#include "TransporterServiceImpl.h"
#include "Utils.h"

#include <iostream>
#include <log4cplus/configurator.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    TransporterApplication::~TransporterApplication() = default;

    void TransporterApplication::Run() {
        EnableLogging();
        const auto worker = getUnique<std::jthread>(&TransporterApplication::StartServer, this);
    }

    void TransporterApplication::EnableLogging() {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
    }

    void TransporterApplication::StartServer() {
        const auto serverManager = getUnique<TransporterServerManager>(
                getShared<TransporterServiceImpl>(),
                SECRET_TRANSPORTER_SERVER_PORT,
                "Secret Transporter");

        serverManager->Init();
    }

} // yakbas