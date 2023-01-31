#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "SecretCommunication.grpc.pb.h"
#include "SealInfo.h"
#include "CustomSealOperations.h"

namespace yakbas::sec {

    class TransporterServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit TransporterServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status StartUsing(grpc::ServerContext *context, const communication::StartUsingRequest *request,
                                communication::sec::StartUsingResponse *response) override;

        grpc::Status EndUsing(::grpc::ServerContext *context, const ::communication::EndUsingRequest *request,
                              ::communication::sec::EndUsingResponse *response) override;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

}// yakbas
