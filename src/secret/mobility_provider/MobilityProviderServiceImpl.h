#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"

namespace yakbas::sec {

    class MobilityProviderServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        MobilityProviderServiceImpl();

        grpc::Status createInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;
    };

}// yakbas
