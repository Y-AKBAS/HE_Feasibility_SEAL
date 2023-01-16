#pragma once

#include <grpcpp/grpcpp.h>
#include "PublicCommunication.grpc.pb.h"

namespace yakbas::pub {

    class ClientServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        ClientServiceImpl();
    };

}// yakbas
