
#include "PlatformServerManager.h"

namespace yakbas::pub {
    PlatformServerManager::PlatformServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                 const std::string &hostAndPort,
                                                 const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort, loggerInstance) {}

} // yakbas