
#include "TransporterServiceImpl.h"

namespace yakbas::sec {

    TransporterServiceImpl::TransporterServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Transporter Service Impl"))) {}

    grpc::Status TransporterServiceImpl::StartUsing(grpc::ServerContext *context,
                                                    const communication::StartUsingRequest *request,
                                                    communication::sec::StartUsingResponse *response) {

        return grpc::Status::OK;
    }

    grpc::Status
    TransporterServiceImpl::EndUsing(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                                     communication::sec::EndUsingResponse *response) {

        return grpc::Status::OK;
    }

} // yakbas