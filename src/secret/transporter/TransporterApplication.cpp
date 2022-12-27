
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
        EnableLogging();
        RunTests(argc, argv);
        this->StartServer();
    }

    void TransporterApplication::StartServer() {
        const auto serverManager = GetUnique<TransporterServerManager>(
                GetShared<TransporterServiceImpl>(),
                SECRET_TRANSPORTER_SERVER_PORT,
                "Secret Transporter Server Manager");

        serverManager->Init();
    }

} // yakbas