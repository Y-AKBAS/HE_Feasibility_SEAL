#pragma once

#include <grpcpp/grpcpp.h>
#include "PublicCommunication.grpc.pb.h"

namespace yakbas {
    namespace pub {

        class InvoiceClerkServiceImpl final : public communication::pub::PublicCommunicationService::Service {

        public:
            InvoiceClerkServiceImpl();

            grpc::Status createInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                       ::communication::InvoicingResponse *response) override;
        };

    } // yakbas
} // pub
