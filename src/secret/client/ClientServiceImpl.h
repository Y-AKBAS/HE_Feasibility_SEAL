#pragma once

#include <grpcpp/grpcpp.h>
#include "SecretCommunication.grpc.pb.h"

namespace yakbas::sec {

    class ClientServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        ClientServiceImpl();

    };

}// yakbas
