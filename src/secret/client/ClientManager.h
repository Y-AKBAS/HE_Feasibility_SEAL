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

        [[nodiscard]] std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
        SearchSecretly(const std::string &from, const std::string &to, int numberOfJourneys = 10);

        [[nodiscard]] std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
        Search(const std::string &from, const std::string &to, int numberOfJourneys = 10);

        [[nodiscard]] std::unique_ptr<communication::sec::BookingResponse>
        BookSecretly(const communication::Journey &journey);

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        BookSecretlyAndDecrypt(const communication::Journey &journey);

        [[nodiscard]] std::unique_ptr<communication::Journey>
        MapSecretToPublic(const communication::sec::Journey &secretJourney);

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        MapSecretToPublic(const communication::sec::BookingResponse &bookingResponse);

        static bool IsInitialized();

    private:
        const std::unique_ptr<SecretUser> m_userPtr{nullptr};
        std::once_flag m_isInitialized;
        std::unique_ptr<log4cplus::Logger> m_logger;
        static std::map<std::string, const std::shared_ptr<seal::PublicKey>> m_publicKeyMap;
    };

} // yakbas
