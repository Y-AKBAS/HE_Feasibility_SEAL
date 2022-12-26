
#include <log4cplus/loggingmacros.h>
#include "ClientManager.h"
#include "ApplicationConstants.h"
#include "ClientGenerator.h"
#include "Utils.h"

namespace yakbas::pub {
    using namespace yakbas::util;

    using publicService = communication::pub::PublicCommunicationService;

    ClientManager::ClientManager()
            : m_userPtr(ClientGenerator::GeneratePublicUser()),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("PublicClientManager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Public Client Manager is being initialized...");
            ClientManager::CreateChannels(
                    {{constants::INVOICE_CLERK_CHANNEL, PUBLIC_INVOICE_CLERK_SERVER_PORT},
                     {constants::PLATFORM_CHANNEL,      PUBLIC_PLATFORM_SERVER_PORT}}
            );
        });
    }

    std::once_flag ClientManager::m_isInitialized{};

    bool ClientManager::IsInitialized() {
        return !m_channelMap.empty();
    }

    std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
    ClientManager::Search(const std::string &from, const std::string &to, int numberOfJourneys) {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);

        auto journeyVecPtr =
                GetUnique<std::vector<std::unique_ptr<communication::Journey>>>();

        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto request = GetUnique<communication::SearchRequest>();

        request->set_numberofjourneys(numberOfJourneys);
        request->set_to(to);
        request->set_from(from);

        const auto readerPtr = stubPtr->SearchForRides(clientContext.get(), *request);

        bool isReadable;
        do {
            auto journeyPtr = GetUnique<communication::Journey>();
            isReadable = readerPtr->Read(journeyPtr.get());

            if (isReadable) {
                journeyVecPtr->push_back(std::move(journeyPtr));
            }
        } while (isReadable);

        const grpc::Status &status = readerPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Fetched Journeys successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Search(). Error message: " + status.error_message());
        }

        return journeyVecPtr;
    }

} // pub