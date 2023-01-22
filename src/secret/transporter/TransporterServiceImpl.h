#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "SecretCommunication.grpc.pb.h"
#include "SealInfo.h"
#include "CustomSealOperations.h"
#include "TransporterClientManager.h"

namespace yakbas::sec {

    class TransporterServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit TransporterServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status StartUsing(grpc::ServerContext *context, const communication::sec::StartUsingRequest *request,
                                communication::StartUsingResponse *response) override;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<TransporterClientManager> m_transporterClientManager{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

}// yakbas
