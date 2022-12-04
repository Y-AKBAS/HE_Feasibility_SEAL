#pragma once

#include "BaseServerManager.h"

namespace yakbas::sec {

    class MobilityProviderServerManager : public BaseServerManager {
    public:
        MobilityProviderServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                      const std::string &hostAndPort,
                                      const std::string &&loggerInstance);
    };

} // yakbas
