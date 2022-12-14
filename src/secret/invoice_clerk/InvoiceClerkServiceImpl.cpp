
#include <log4cplus/loggingmacros.h>
#include "InvoiceClerkServiceImpl.h"

namespace yakbas::sec {
    InvoiceClerkServiceImpl::InvoiceClerkServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Invoice Clerk Service Impl"))) {}

    grpc::Status
    InvoiceClerkServiceImpl::GetPublicKey(grpc::ServerContext *context, const google::protobuf::Empty *request,
                                          communication::sec::PublicKey *response) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Invoice Clerk Public Key requested ...");
        response->set_publickey(m_customSealOperationsPtr->GetPublicKeyBuffer());
        return grpc::Status::OK;
    }

    grpc::Status InvoiceClerkServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                        const communication::InvoicingRequest *request,
                                                        communication::InvoicingResponse *response) {

        // Normally the invoice should be created here :)

        response->set_status(communication::StatusCode::SUCCESSFUL);
        return grpc::Status::OK;
    }

} // yakbas