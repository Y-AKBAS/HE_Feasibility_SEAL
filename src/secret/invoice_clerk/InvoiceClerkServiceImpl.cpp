
#include "InvoiceClerkServiceImpl.h"

namespace yakbas::sec {
    InvoiceClerkServiceImpl::InvoiceClerkServiceImpl()
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>()) {}

    grpc::Status InvoiceClerkServiceImpl::createInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }

    grpc::Status
    InvoiceClerkServiceImpl::getPublicKey(grpc::ServerContext *context, const google::protobuf::Empty *request,
                                          communication::sec::PublicKey *response) {


        return Service::getPublicKey(context, request, response);
    }

} // yakbas