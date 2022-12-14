#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"
#include "CustomSealOperations.h"

namespace yakbas::sec {

    class InvoiceClerkServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        InvoiceClerkServiceImpl();

        grpc::Status createInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;

        grpc::Status getPublicKey(grpc::ServerContext *context, const google::protobuf::Empty *request,
                                  communication::sec::PublicKey *response) override;
    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
    };

}// yakbas
