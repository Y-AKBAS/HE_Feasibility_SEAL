
#include <log4cplus/loggingmacros.h>
#include "PlatformServiceImpl.h"
#include "ApplicationConstants.h"

namespace yakbas::sec {

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

    grpc::Status
    PlatformServiceImpl::searchForRides(grpc::ServerContext *context, const communication::sec::SearchRequest *request,
                                        communication::sec::SearchResponse *response) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Service impl SearchForRides invoked...");

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL);
        grpc::ClientContext clientContext;
        communication::sec::SearchResponse myResponse;
        const auto status = stubPtr->searchForRides(&clientContext, *request, &(*response));
        //response = &myResponse;
        return status;
    }

} // yakbas