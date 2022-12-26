
#include <log4cplus/loggingmacros.h>
#include "MobilityProviderServiceImpl.h"
#include "MobilityProviderGenerator.h"

namespace yakbas::pub {
    MobilityProviderServiceImpl::MobilityProviderServiceImpl()
            : m_logger(std::make_unique<log4cplus::Logger>(
            log4cplus::Logger::getInstance("Public Mobility Provider Service Impl"))) {}

    grpc::Status MobilityProviderServiceImpl::SearchForRides(grpc::ServerContext *context,
                                                             const communication::SearchRequest *request,
                                                             grpc::ServerWriter<communication::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Public Mobility Provider Service impl SearchForRides invoked...");
        return MobilityProviderGenerator::GenerateJourneys(request, writer);
    }
} // yakbas