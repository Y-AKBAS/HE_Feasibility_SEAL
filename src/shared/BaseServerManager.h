#pragma once

#include <memory>
#include <log4cplus/logger.h>
#include <grpcpp/grpcpp.h>

namespace yakbas {

    class BaseServerManager {
    public:
        BaseServerManager(std::shared_ptr<grpc::Service> serverImpl, std::string hostAndPort);

    protected:
        void Init();
        std::unique_ptr<log4cplus::Logger> m_logger;
        std::unique_ptr<grpc::Server> m_server;
        std::shared_ptr<grpc::Service> m_serverImpl;
        const std::string m_hostAndPort;
    };

}