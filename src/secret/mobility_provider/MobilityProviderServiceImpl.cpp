
#include <log4cplus/loggingmacros.h>
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

        LOG4CPLUS_DEBUG(*m_logger, "Secret Mobility Provider Service impl SearchForRides invoked...");

        const int numberOfJourneys = 1;
        const auto stream = GetUniqueStream(request->publickey());
        const auto publicKeyPtr = m_customSealOperationsPtr->GetPublicKeyFromBuffer(stream);
        const auto newEncryptorPtr = CustomSealOperations::CreateNewEncryptor(*publicKeyPtr);

        auto status = MobilityProviderGenerator::GenerateJourneys(request, &(*response), *newEncryptorPtr,
                                                                  numberOfJourneys);
        return status;
    }

} // yakbas