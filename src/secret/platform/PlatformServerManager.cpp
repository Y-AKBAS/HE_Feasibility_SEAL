
#include "PlatformServerManager.h"

namespace yakbas::sec {
    PlatformServerManager::PlatformServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                 const std::string &hostAndPort,
                                                 const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort) {}

} // yakbas