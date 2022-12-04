
#include "TransporterServiceImpl.h"

namespace yakbas::sec {
    TransporterServiceImpl::TransporterServiceImpl() = default;

    grpc::Status TransporterServiceImpl::createInvoice(grpc::ServerContext *context,
                                                       const communication::InvoicingRequest *request,
                                                       communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }
} // yakbas