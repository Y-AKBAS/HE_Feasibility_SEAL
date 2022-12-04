#pragma once

#include "BaseServerManager.h"

namespace yakbas::sec {

    class PlatformServerManager : public BaseServerManager {
    public:
        PlatformServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                              const std::string &hostAndPort,
                              const std::string &&loggerInstance);
    };

} // yakbas
