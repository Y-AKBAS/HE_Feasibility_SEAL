
#include "MobilityProviderServiceImpl.h"

namespace yakbas::pub {
    MobilityProviderServiceImpl::MobilityProviderServiceImpl() = default;

    grpc::Status MobilityProviderServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                            const communication::InvoicingRequest *request,
                                                            communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::CreateInvoice(context, request, response);
    }
} // yakbas