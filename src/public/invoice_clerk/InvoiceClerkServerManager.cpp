
#include "InvoiceClerkServerManager.h"

namespace yakbas::pub {
    InvoiceClerkServerManager::InvoiceClerkServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                         const std::string &hostAndPort,
                                                         const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort) {}

} // yakbas