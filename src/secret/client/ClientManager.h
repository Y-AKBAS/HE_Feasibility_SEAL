#pragma once

#include "SharedCommunication.pb.h"
#include "SecretCommunication.grpc.pb.h"
#include "SecretUser.h"
#include "SecretBaseClientManager.h"

namespace yakbas::sec {

    class ClientManager : protected SecretBaseClientManager {
    public:
        explicit ClientManager(const SealKeys &sealKeys = {});

        ~ClientManager() override;

        void GetPublicKey() const override;

        [[nodiscard]] std::unique_ptr<communication::SearchResponse>
        DoSearchRequest(const std::string &from, const std::string &to);

        [[nodiscard]] std::unique_ptr<communication::SearchResponse>
        MapSecretToPublic(const google::protobuf::RepeatedPtrField<communication::sec::Journey> &journeysPtr);

        static bool IsInitialized();

    private:
        const std::unique_ptr<SecretUser> m_userPtr{nullptr};
        std::once_flag m_isInitialized;
        std::unique_ptr<log4cplus::Logger> m_logger;
        static std::map<std::string, const std::shared_ptr<seal::PublicKey>> m_publicKeyMap;
    };

} // yakbas
