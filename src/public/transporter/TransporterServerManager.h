#pragma once

#include "BaseServerManager.h"

namespace yakbas::pub {

    class TransporterServerManager : public BaseServerManager {
    public:
        TransporterServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                 const std::string &hostAndPort,
                                 const std::string &&loggerInstance);
    };

} // yakbas
