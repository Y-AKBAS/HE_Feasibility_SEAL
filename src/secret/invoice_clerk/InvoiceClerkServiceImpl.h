#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"

namespace yakbas::sec {

    class InvoiceClerkServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        InvoiceClerkServiceImpl();

        grpc::Status createInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;
    };

}// yakbas
