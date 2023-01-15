
#include "ClientApplication.h"
#include "ClientServerManager.h"
#include "ClientServiceImpl.h"
#include "Utils.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    ClientApplication::~ClientApplication() = default;

    void ClientApplication::Run(int argc, char **argv) {
        try {
            EnableLogging();
            RunTests(argc, argv);
            auto commandLinePtr = HandleCommandLine(argc, argv, "Secret Client Application");
            this->StartServer(commandLinePtr.get());
        } catch (std::exception &e) {
            const auto logger = log4cplus::Logger::getInstance("Secret Client Exception Logger");
            LOG4CPLUS_ERROR(logger, std::string("Exception message: ") + e.what());
            DisableLogging();
        }
    }

    void ClientApplication::StartServer(BaseCommandLineInfo *commandLineInfoPtr) {
        const auto &secretCmdLineInfoPtr = reinterpret_cast<SecretCommandLineInfo *>(commandLineInfoPtr);

        if (secretCmdLineInfoPtr == nullptr) {
            throw std::bad_cast();
        }
        const auto serverManager = GetUnique<ClientServerManager>(
                GetShared<ClientServiceImpl>(secretCmdLineInfoPtr->m_sealKeys),
                SECRET_CLIENT_SERVER_PORT,
                "Secret Client Server Manager");

        serverManager->Init();
    }

} // yakbas