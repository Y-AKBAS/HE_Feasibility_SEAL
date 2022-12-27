
#include "TransporterServerManager.h"

namespace yakbas::pub {
    TransporterServerManager::TransporterServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                       const std::string &hostAndPort,
                                                       const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort, loggerInstance) {}

} // yakbas