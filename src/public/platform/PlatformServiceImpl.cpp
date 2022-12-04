
#include "PlatformServiceImpl.h"

namespace yakbas::pub {
    PlatformServiceImpl::PlatformServiceImpl() = default;

    grpc::Status PlatformServiceImpl::createInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }
} // yakbas