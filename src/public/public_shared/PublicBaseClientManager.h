#pragma once

#include "BaseClientManager.h"
#include "PublicCommunication.grpc.pb.h"

namespace yakbas::pub {

    class PublicBaseClientManager : protected BaseClientManager {

    public:

        using publicService = communication::pub::PublicCommunicationService;

        PublicBaseClientManager();

        ~PublicBaseClientManager() override;

        [[nodiscard]] std::unique_ptr<publicService::Stub> GetStub(const std::string &channelName) const;

    };

} // yakbas
