#pragma once

#include <grpcpp/grpcpp.h>
#include "PublicCommunication.grpc.pb.h"

namespace yakbas::pub {

    class MobilityProviderServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        MobilityProviderServiceImpl();

        grpc::Status CreateInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;
    };

}// yakbas
