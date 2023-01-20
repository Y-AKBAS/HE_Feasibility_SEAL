
#include <utility>
#include <log4cplus/loggingmacros.h>
#include <thread>
#include <chrono>

#include "BaseServerManager.h"

namespace yakbas {

    BaseServerManager::BaseServerManager(std::shared_ptr<grpc::Service> serverImpl,
                                         std::string hostAndPort,
                                         const std::string &loggerInstance)
            : m_serverImpl(std::move(serverImpl)),
              m_hostAndPort(std::move(hostAndPort)),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance(loggerInstance))) {}

    void BaseServerManager::Init() {
        LOG4CPLUS_DEBUG(*m_logger, "Starting server on host:port: " + m_hostAndPort);
        std::unique_ptr<grpc::ServerBuilder> builder = std::make_unique<grpc::ServerBuilder>();
        builder->AddListeningPort(m_hostAndPort, grpc::InsecureServerCredentials());
        builder->RegisterService(m_serverImpl.get());
        builder->SetMaxSendMessageSize(std::numeric_limits<int32_t>::max());
        builder->SetMaxReceiveMessageSize(std::numeric_limits<int32_t>::max());
        m_server = builder->BuildAndStart();
        m_server->Wait();
    }

    void BaseServerManager::Shutdown() {
        if (m_server != nullptr) {
            m_server->Shutdown();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

} // yakbas
