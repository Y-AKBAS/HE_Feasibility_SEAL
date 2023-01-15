#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"
#include "SealInfo.h"

namespace yakbas::sec {

    class ClientServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit ClientServiceImpl(const SealKeys &sealKeys = {});

        ~ClientServiceImpl() override;
    };

}// yakbas
