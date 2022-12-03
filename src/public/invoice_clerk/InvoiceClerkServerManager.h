#pragma once

#include "BaseServerManager.h"

namespace yakbas {
    namespace pub {

        class InvoiceClerkServerManager : public BaseServerManager {
        public:
            InvoiceClerkServerManager(const std::shared_ptr<grpc::Service> &serverImpl,
                                      const std::string &hostAndPort,
                                      const std::string &&loggerInstance);
        };

    } // yakbas
} // pub
