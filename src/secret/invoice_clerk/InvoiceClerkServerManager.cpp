
#include "InvoiceClerkServerManager.h"

namespace yakbas::sec {
    InvoiceClerkServerManager::InvoiceClerkServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                                         const std::string &hostAndPort,
                                                         const std::string &&loggerInstance)
            : BaseServerManager(serverImpl, hostAndPort) {}

} // yakbas