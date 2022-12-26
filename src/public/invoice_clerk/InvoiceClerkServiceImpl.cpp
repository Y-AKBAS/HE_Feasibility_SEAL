
#include "InvoiceClerkServiceImpl.h"

namespace yakbas::pub {
    InvoiceClerkServiceImpl::InvoiceClerkServiceImpl() = default;

    grpc::Status InvoiceClerkServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::CreateInvoice(context, request, response);
    }
} // yakbas