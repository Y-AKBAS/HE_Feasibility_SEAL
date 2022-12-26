
#include "PlatformServiceImpl.h"

namespace yakbas::pub {
    PlatformServiceImpl::PlatformServiceImpl() = default;

    grpc::Status PlatformServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                    const communication::InvoicingRequest *request,
                                                    communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::CreateInvoice(context, request, response);
    }
} // yakbas