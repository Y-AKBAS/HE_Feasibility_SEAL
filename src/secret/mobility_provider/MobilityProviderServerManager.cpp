
#include "MobilityProviderServerManager.h"

namespace yakbas::sec {
    MobilityProviderServerManager::MobilityProviderServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                                 const std::string &hostAndPort,
                                                                 const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort) {}

} // yakbas