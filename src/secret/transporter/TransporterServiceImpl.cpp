
#include "TransporterServiceImpl.h"

namespace yakbas::sec {

    TransporterServiceImpl::TransporterServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Transporter Service Impl"))) {}

    grpc::Status TransporterServiceImpl::createInvoice(grpc::ServerContext *context,
                                                       const communication::InvoicingRequest *request,
                                                       communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }

} // yakbas