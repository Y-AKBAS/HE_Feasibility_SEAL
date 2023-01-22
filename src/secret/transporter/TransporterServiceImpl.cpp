
#include "TransporterServiceImpl.h"

namespace yakbas::sec {

    TransporterServiceImpl::TransporterServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_transporterClientManager(std::make_unique<TransporterClientManager>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Transporter Service Impl"))) {}

    grpc::Status TransporterServiceImpl::StartUsing(grpc::ServerContext *context,
                                                    const communication::sec::StartUsingRequest *request,
                                                    communication::StartUsingResponse *response) {

        grpc::ClientContext clientContext{};
        const std::string transporterId = GetUUID();
        auto updatedRequest = GetUnique<communication::sec::StartUsingRequest>();
        updatedRequest->CopyFrom(*request);
        updatedRequest->set_transporter_id(transporterId);

        return m_transporterClientManager->GetStub(constants::PLATFORM_CHANNEL)->StartUsing(&clientContext,
                                                                                            *updatedRequest,
                                                                                            response);
    }

} // yakbas