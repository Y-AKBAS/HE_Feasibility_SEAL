
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
            ClientManager::CreateChannels();
            this->GetPublicKey();
        });
    }

    std::map<std::string, const std::shared_ptr<seal::PublicKey>> ClientManager::m_publicKeyMap{};

    auto ClientManager::GetStub(const std::string &channelName) {

        const auto pair = m_channelMap.find(channelName);

        if (pair != m_channelMap.end()) {
            return communication::sec::SecretCommunicationService::NewStub(pair->second);
        }

        throw std::invalid_argument("No Such Channel");
    }

    void ClientManager::GetPublicKey() const {

        const auto invoiceClerkStubPtr = GetStub(constants::INVOICE_CLERK_CHANNEL);

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

    void ClientManager::CreateChannels() {
        if (m_channelMap.empty()) {
            m_channelMap.insert(
                    std::make_pair(constants::INVOICE_CLERK_CHANNEL,
                                   grpc::CreateChannel(SECRET_INVOICE_CLERK_SERVER_PORT,
                                                       grpc::InsecureChannelCredentials())));
            m_channelMap.insert(
                    std::make_pair(constants::PLATFORM_CHANNEL,
                                   grpc::CreateChannel(SECRET_PLATFORM_SERVER_PORT,
                                                       grpc::InsecureChannelCredentials())));
        }
    }

    bool ClientManager::IsInitialized() {
        return !m_publicKeyMap.empty() && !m_channelMap.empty();
    }

} // yakbas