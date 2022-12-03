
#include "ClientServiceImpl.h"

namespace yakbas::pub {
    ClientServiceImpl::ClientServiceImpl() = default;

    grpc::Status ClientServiceImpl::createInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }
} // yakbas