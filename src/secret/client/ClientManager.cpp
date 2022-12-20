
#include <log4cplus/loggingmacros.h>
#include "ClientManager.h"
#include "ClientGenerator.h"
#include "ApplicationConstants.h"
#include "SecretCommunication.pb.h"
#include "Utils.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    ClientManager::~ClientManager() = default;

    ClientManager::ClientManager(const SealKeys &sealKeys)
            : m_userPtr(ClientGenerator::GenerateSecretUser(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("SecretClientManager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Secret Client Manager is being initialized...");
            ClientManager::CreateChannels(
                    {{constants::INVOICE_CLERK_CHANNEL, SECRET_INVOICE_CLERK_SERVER_PORT},
                     {constants::PLATFORM_CHANNEL,      SECRET_PLATFORM_SERVER_PORT}}
            );
            this->GetPublicKey();
        });
    }

    std::map<std::string, const std::shared_ptr<seal::PublicKey>> ClientManager::m_publicKeyMap{};

    void ClientManager::GetPublicKey() const {

        const std::unique_ptr<secretService::Stub> invoiceClerkStubPtr
                = this->GetStub(constants::INVOICE_CLERK_CHANNEL);

        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto request = GetUnique<google::protobuf::Empty>();
        const auto response = GetUnique<communication::sec::PublicKey>();

        invoiceClerkStubPtr->getPublicKey(clientContext.get(), *request, response.get());

        auto invoiceClerkPublicKey = m_userPtr->GetCustomSealOperations()->GetPublicKeyFromBuffer(
                GetUniqueStream(response->publickey()));

        LOG4CPLUS_DEBUG(*m_logger, "Secret Client Manager requested public key...");

        std::shared_ptr<seal::PublicKey> key(invoiceClerkPublicKey.release());
        m_publicKeyMap.insert(std::make_pair(constants::INVOICE_CLERK_CHANNEL, key));
    }

    bool ClientManager::IsInitialized() {
        return !m_publicKeyMap.empty() && !m_channelMap.empty();
    }

    std::unique_ptr<communication::SearchResponse>
    ClientManager::DoSearchRequest(const std::string &from, const std::string &to) {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);

        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto request = GetUnique<communication::sec::SearchRequest>();
        const auto response = GetUnique<communication::sec::SearchResponse>();

        request->set_to(to);
        request->set_from(from);
        request->set_publickey(m_userPtr->GetCustomSealOperations()->GetPublicKeyBuffer());

        stubPtr->searchForRides(clientContext.get(), *request, response.get());

        return this->MapSecretToPublic(response->journeys());
    }

    std::unique_ptr<communication::SearchResponse>
    ClientManager::MapSecretToPublic(
            const google::protobuf::RepeatedPtrField<communication::sec::Journey> &journeysPtr) {

        auto publicJourneysPtr = GetUnique<communication::SearchResponse>();

        for (const auto &journey: journeysPtr) {

            const auto &ridesPtr = journey.rides();
            auto publicJourney = publicJourneysPtr->add_journeys();

            for (const auto &ride: ridesPtr) {

                const auto publicRidePtr = publicJourney->add_rides();

                publicRidePtr->set_from(ride.from());
                publicRidePtr->set_to(ride.to());
                publicRidePtr->set_providerid(ride.providerid());
                publicRidePtr->set_rideid(ride.rideid());

               /* const uint64_t discountRate = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(ride.discountrate()));
                publicRidePtr->set_discountrate(discountRate);*/

                const uint64_t coefficient = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(ride.coefficient()));
                publicRidePtr->set_coefficient(coefficient);
                std::cout << "coefficient: " << coefficient << std::endl;

                const auto timestampPtr = publicRidePtr->mutable_starttime();
                timestampPtr->set_seconds(ride.starttime().seconds());
                timestampPtr->set_nanos(ride.starttime().nanos());

                const auto transporterPtr = publicRidePtr->mutable_transporter();
                transporterPtr->set_providerid(ride.transporter().providerid());
                transporterPtr->set_unitpricetype(ride.transporter().unitpricetype());
                transporterPtr->set_transportertype(ride.transporter().transportertype());
                transporterPtr->set_capacity(ride.transporter().capacity());

               /* const uint64_t seatPrice = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(ride.transporter().seatprice()));
                transporterPtr->set_seatprice(seatPrice);*/

                const uint64_t unitPrice = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(ride.transporter().unitprice()));
                transporterPtr->set_unitprice(unitPrice);
                std::cout << "unitPrice: " << unitPrice << std::endl;


            }
        }

        return publicJourneysPtr;
    }

} // yakbas