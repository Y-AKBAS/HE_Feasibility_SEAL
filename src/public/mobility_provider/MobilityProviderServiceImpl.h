#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "PublicCommunication.grpc.pb.h"

namespace yakbas::pub {

    class MobilityProviderServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        MobilityProviderServiceImpl();

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<::communication::Journey> *writer) override;

        grpc::Status ReportInvoicing(::grpc::ServerContext *context, const ::communication::InvoicingReport *request,
                                     ::communication::InvoicingResponse *response) override;

    private:
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

}// yakbas
