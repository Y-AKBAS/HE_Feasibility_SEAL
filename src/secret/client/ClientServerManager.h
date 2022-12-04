#pragma once

#include "BaseServerManager.h"

namespace yakbas::sec {

    class ClientServerManager : public BaseServerManager {
    public:
        ClientServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                            const std::string &hostAndPort,
                            const std::string &&loggerInstance);
    };

} // yakbas
