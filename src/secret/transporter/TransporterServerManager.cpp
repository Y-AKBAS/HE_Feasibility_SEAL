
#include "TransporterServerManager.h"

namespace yakbas::sec {
    TransporterServerManager::TransporterServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                       const std::string &hostAndPort,
                                                       const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort) {}

} // yakbas