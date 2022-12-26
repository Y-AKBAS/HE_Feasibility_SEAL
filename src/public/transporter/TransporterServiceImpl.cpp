
#include "TransporterServiceImpl.h"

namespace yakbas::pub {
    TransporterServiceImpl::TransporterServiceImpl() = default;

    grpc::Status TransporterServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                       const communication::InvoicingRequest *request,
                                                       communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::CreateInvoice(context, request, response);
    }
} // yakbas