
#include "PlatformServiceImpl.h"

namespace yakbas::sec {

    PlatformServiceImpl::PlatformServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
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


        return {};
    }

} // yakbas