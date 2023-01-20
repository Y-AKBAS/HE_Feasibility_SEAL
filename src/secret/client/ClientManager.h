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
        SearchSecretly(const std::string &from, const std::string &to, int numberOfJourneys = 10) const;

        [[nodiscard]] std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
        Search(const std::string &from, const std::string &to, int numberOfJourneys = 10) const;

        [[nodiscard]] std::unique_ptr<communication::sec::BookingResponse>
        BookSecretly(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::sec::BookingResponse>
        BookSymmetricSecretly(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        BookSecretlyAndDecrypt(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        BookSymmetricSecretlyAndDecrypt(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::sec::BookingResponse>
        BookSymmetricSecretlyOnOthers(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        BookSymmetricSecretlyOnOthersAndDecrypt(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::sec::BookingResponse>
        BookSecretlyOnOthers(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        BookSecretlyOnOthersAndDecrypt(const communication::Journey &journey) const;

        [[nodiscard]] std::unique_ptr<communication::InvoicingResponse>
        Pay(const communication::BookingResponse &bookingResponse) const;

        [[nodiscard]] communication::StatusCode
        ReportInvoicing(const communication::InvoicingResponse &invoicingResponse,
                        const communication::BookingResponse &bookingResponse) const;

        [[nodiscard]] seal::scheme_type GetSchemeType() const;

        static bool IsInitialized();

    private:

        [[nodiscard]] std::unique_ptr<communication::Journey>
        MapSecretToPublic(const communication::sec::Journey &secretJourney) const;

        [[nodiscard]] std::unique_ptr<communication::BookingResponse>
        MapSecretToPublic(const communication::sec::BookingResponse &bookingResponse) const;

        static void MapRideAndSeatNumberMap(google::protobuf::Map<std::string, int32_t> &targetMap,
                                            const google::protobuf::Map<std::string, int32_t> *sourceMapPtr);

        const std::unique_ptr<SecretUser> m_userPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        seal::scheme_type m_schemeType;

    private:

        static std::map<std::string, const std::shared_ptr<seal::PublicKey>> m_publicKeyMap;
        static std::once_flag m_isInitialized;

    };

} // yakbas
