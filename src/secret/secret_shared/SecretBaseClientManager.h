#pragma once

#include "BaseClientManager.h"
#include "SecretCommunication.grpc.pb.h"

namespace yakbas::sec {

    class SecretBaseClientManager : protected BaseClientManager {

    public:
        using secretService = communication::sec::SecretCommunicationService;

        SecretBaseClientManager();

        ~SecretBaseClientManager() override;

        virtual void GetPublicKey() const = 0;

        [[nodiscard]] std::unique_ptr<secretService::Stub> GetStub(const std::string &channelName) const;

    protected:
    private:
    };

} // yakbas
