
#include "InvoiceClerkServiceImpl.h"

namespace yakbas::pub {
    InvoiceClerkServiceImpl::InvoiceClerkServiceImpl() = default;

    grpc::Status InvoiceClerkServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Normally the invoice should be created here :)

        response->set_status(communication::StatusCode::SUCCESSFUL);
        return grpc::Status::OK;
    }

} // yakbas