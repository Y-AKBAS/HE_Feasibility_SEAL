
#include <log4cplus/loggingmacros.h>
#include "InvoiceClerkServiceImpl.h"

namespace yakbas::sec {
    InvoiceClerkServiceImpl::InvoiceClerkServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Invoice Clerk Service Impl"))) {}

    grpc::Status InvoiceClerkServiceImpl::createInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::createInvoice(context, request, response);
    }

    grpc::Status
    InvoiceClerkServiceImpl::getPublicKey(grpc::ServerContext *context, const google::protobuf::Empty *request,
                                          communication::sec::PublicKey *response) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Invoice Clerk Public Key requested ...");
        const auto &stream = m_customSealOperationsPtr->PublicKeyToBuffer();
        response->set_publickey(stream->str());
        return {};
    }

} // yakbas