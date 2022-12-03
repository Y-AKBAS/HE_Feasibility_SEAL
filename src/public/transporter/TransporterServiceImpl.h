#pragma once

#include <grpcpp/grpcpp.h>
#include "PublicCommunication.grpc.pb.h"

namespace yakbas::pub {

    class TransporterServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        TransporterServiceImpl();

        grpc::Status createInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;
    };

}// yakbas
