#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "PublicCommunication.grpc.pb.h"
#include "PlatformClientManager.h"

namespace yakbas::pub {

    class PlatformServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        PlatformServiceImpl();

        grpc::Status CreateInvoice(::grpc::ServerContext *context, const ::communication::InvoicingRequest *request,
                                   ::communication::InvoicingResponse *response) override;

        grpc::Status SearchForRides(::grpc::ServerContext *context, const ::communication::SearchRequest *request,
                                    ::grpc::ServerWriter<::communication::Journey> *writer) override;

    private:
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const std::unique_ptr<PlatformClientManager> m_platformClientManager{nullptr};
    };


}// yakbas
