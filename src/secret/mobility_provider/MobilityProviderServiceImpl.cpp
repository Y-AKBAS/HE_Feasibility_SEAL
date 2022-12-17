
#include "MobilityProviderServiceImpl.h"
#include "Utils.h"
#include "MobilityProviderGenerator.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    MobilityProviderServiceImpl::MobilityProviderServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Mobility Provider Service Impl"))) {}


    grpc::Status MobilityProviderServiceImpl::createInvoice(grpc::ServerContext *context,
                                                            const communication::InvoicingRequest *request,
                                                            communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }

    grpc::Status MobilityProviderServiceImpl::searchForRides(grpc::ServerContext *context,
                                                             const communication::sec::SearchRequest *request,
                                                             communication::sec::SearchResponse *response) {

        const int numberOfJourneys = 10;
        const auto stream = GetUniqueStream(request->publickey());
        const auto publicKeyPtr = m_customSealOperationsPtr->GetPublicKeyFromBuffer(stream);
        const auto newEncryptorPtr = CustomSealOperations::CreateNewEncryptor(*publicKeyPtr);

        return MobilityProviderGenerator::GenerateJourneys(request, response, *newEncryptorPtr, numberOfJourneys);
    }

} // yakbas