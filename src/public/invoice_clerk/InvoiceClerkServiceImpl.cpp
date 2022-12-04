
#include "InvoiceClerkServiceImpl.h"

namespace yakbas::pub {
    InvoiceClerkServiceImpl::InvoiceClerkServiceImpl() = default;

    grpc::Status InvoiceClerkServiceImpl::createInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }
} // yakbas