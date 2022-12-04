
#include "ClientServerManager.h"

namespace yakbas::pub {
    ClientServerManager::ClientServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                         const std::string &hostAndPort,
                                                         const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort) {}

} // yakbas