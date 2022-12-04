#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"

namespace yakbas::sec {

    class PlatformServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        PlatformServiceImpl();

        grpc::Status createInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;
    };

}// yakbas
