#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "PublicCommunication.grpc.pb.h"
#include "PlatformClientManager.h"

namespace yakbas::pub {

    class PlatformServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        PlatformServiceImpl();

        grpc::Status CreateInvoice(grpc::ServerContext *context, const communication::InvoicingRequest *request,
                                   communication::InvoicingResponse *response) override;

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<communication::Journey> *writer) override;

        grpc::Status
        Book(::grpc::ServerContext *context, ::grpc::ServerReader<::communication::pub::BookingRequest> *reader,
             ::communication::BookingResponse *response) override;

        grpc::Status ReportInvoicing(grpc::ServerContext *context, const communication::InvoicingReport *request,
                                     communication::InvoicingResponse *response) override;

    private:
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const std::unique_ptr<PlatformClientManager> m_platformClientManager{nullptr};

        [[nodiscard]] static std::uint64_t
        GetRequestTotalAndInsertSeat(const communication::pub::BookingRequest &request,
                                     google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap);

    };


}// yakbas
