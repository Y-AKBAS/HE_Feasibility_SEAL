
#include <utility>
#include <thread>
#include <log4cplus/loggingmacros.h>

#include "BaseServerManager.h"

namespace yakbas {

    BaseServerManager::BaseServerManager(std::shared_ptr<grpc::Service> serverImpl, std::string hostAndPort)
            : m_serverImpl(std::move(serverImpl)), m_hostAndPort(std::move(hostAndPort)) {}

    void BaseServerManager::Init() {
        LOG4CPLUS_DEBUG(*m_logger, "Starting server on host:port: " + m_hostAndPort);
        std::unique_ptr<grpc::ServerBuilder> builder = std::make_unique<grpc::ServerBuilder>();
        builder->AddListeningPort(m_hostAndPort, grpc::InsecureServerCredentials());
        builder->RegisterService(m_serverImpl.get());
        m_server = builder->BuildAndStart();
        std::thread worker(&grpc::Server::Wait, m_server.get());
        worker.join();
        LOG4CPLUS_DEBUG(*m_logger, "Started server on host:port: " + m_hostAndPort);
    }

} // yakbas
