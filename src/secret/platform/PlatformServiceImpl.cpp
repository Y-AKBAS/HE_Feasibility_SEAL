
#include <log4cplus/loggingmacros.h>
#include "PlatformServiceImpl.h"
#include "ApplicationConstants.h"
#include "Utils.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    PlatformServiceImpl::PlatformServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_platformClientManager(std::make_unique<PlatformClientManager>()),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Platform Service Impl"))) {}

    grpc::Status PlatformServiceImpl::createInvoice(grpc::ServerContext *context,
                                                    const communication::InvoicingRequest *request,
                                                    communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }

    grpc::Status PlatformServiceImpl::SearchForSecretRides(grpc::ServerContext *context,
                                                           const communication::sec::SearchRequest *request,
                                                           grpc::ServerWriter<communication::sec::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Service impl SearchForSecretRides invoked...");

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL);
        grpc::ClientContext clientContext;
        const auto clientReaderPtr = stubPtr->SearchForSecretRides(&clientContext, *request);

        bool isReadable;
        do {
            const auto journeyPtr = GetUnique<communication::sec::Journey>();
            isReadable = clientReaderPtr->Read(journeyPtr.get());
            if (isReadable) {
                writer->Write(*journeyPtr);
            }
        } while (isReadable);

        const grpc::Status &status = clientReaderPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Journeys sent successfully ... ");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during SearchForSecretRides(). Error message: " + status.error_message());
        }

        return status;

    }

    grpc::Status PlatformServiceImpl::SearchForRides(grpc::ServerContext *context,
                                                     const communication::SearchRequest *request,
                                                     grpc::ServerWriter<communication::Journey> *writer) {


        LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Service impl SearchForRides invoked...");

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL);
        grpc::ClientContext clientContext;
        const auto clientReaderPtr = stubPtr->SearchForRides(&clientContext, *request);

        bool isReadable;
        do {
            const auto journeyPtr = GetUnique<communication::Journey>();
            isReadable = clientReaderPtr->Read(journeyPtr.get());
            if (isReadable) {
                writer->Write(*journeyPtr);
            }
        } while (isReadable);

        const grpc::Status &status = clientReaderPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Journeys sent successfully ... ");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during SearchForRides(). Error message: " + status.error_message());
        }

        return status;
    }

} // yakbas