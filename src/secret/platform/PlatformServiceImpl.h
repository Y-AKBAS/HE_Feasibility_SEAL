#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "SecretCommunication.grpc.pb.h"
#include "CustomSealOperations.h"
#include "PlatformClientManager.h"

namespace yakbas::sec {

    class PlatformServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit PlatformServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status createInvoice(grpc::ServerContext *context, const communication::InvoicingRequest *request,
                                   communication::InvoicingResponse *response) override;

        grpc::Status
        SearchForSecretRides(grpc::ServerContext *context, const communication::sec::SearchRequest *request,
                             grpc::ServerWriter<communication::sec::Journey> *writer) override;

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<communication::Journey> *writer) override;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const std::unique_ptr<PlatformClientManager> m_platformClientManager{nullptr};
    };

}// yakbas
