#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"
#include "CustomSealOperations.h"

namespace yakbas::sec {

    class InvoiceClerkServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit InvoiceClerkServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status GetPublicKey(grpc::ServerContext *context, const google::protobuf::Empty *request,
                                  communication::sec::PublicKey *response) override;

        grpc::Status CreateInvoice(grpc::ServerContext *context, const communication::InvoicingRequest *request,
                                   communication::InvoicingResponse *response) override;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

}// yakbas
