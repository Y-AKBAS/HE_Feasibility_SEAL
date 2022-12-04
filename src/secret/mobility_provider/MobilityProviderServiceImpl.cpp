
#include "MobilityProviderServiceImpl.h"

namespace yakbas::sec {
    MobilityProviderServiceImpl::MobilityProviderServiceImpl() = default;

    grpc::Status MobilityProviderServiceImpl::createInvoice(grpc::ServerContext *context,
                                                            const communication::InvoicingRequest *request,
                                                            communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }
} // yakbas