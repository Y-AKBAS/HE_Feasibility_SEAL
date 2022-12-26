#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "SecretCommunication.grpc.pb.h"
#include "CustomSealOperations.h"

namespace yakbas::sec {

    class MobilityProviderServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit MobilityProviderServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status
        SearchForSecretRides(grpc::ServerContext *context, const communication::sec::SearchRequest *request,
                             grpc::ServerWriter<communication::sec::Journey> *writer) override;

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<communication::Journey> *writer) override;

        grpc::Status ReportInvoicing(::grpc::ServerContext *context, const ::communication::InvoicingReport *request,
                                     ::communication::InvoicingResponse *response) override;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

}// yakbas
