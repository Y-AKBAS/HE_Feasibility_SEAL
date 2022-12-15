#pragma once

#include "BaseClientManager.h"
#include "SharedCommunication.pb.h"
#include "SecretCommunication.grpc.pb.h"
#include "SecretUser.h"

namespace yakbas::sec {

    class ClientManager : protected BaseClientManager {
    public:
        explicit ClientManager(const SealKeys &sealKeys = {});

        ~ClientManager() override;

        static auto GetStub(const std::string &channelName);

        void GetPublicKey() const;

        static bool IsInitialized();

        static void CreateChannels();

    private:
        const std::unique_ptr<SecretUser> m_userPtr{nullptr};
        std::once_flag m_isInitialized;
        std::unique_ptr<log4cplus::Logger> m_logger;
        static std::map<std::string, const std::shared_ptr<seal::PublicKey>> m_publicKeyMap;
    };

} // yakbas
