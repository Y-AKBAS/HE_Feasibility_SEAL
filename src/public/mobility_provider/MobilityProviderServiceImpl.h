#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "PublicCommunication.grpc.pb.h"
#include "MobilityProviderClientManager.h"

namespace yakbas::pub {

    class MobilityProviderServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        explicit MobilityProviderServiceImpl(const std::string &&loggerInstance);

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<::communication::Journey> *writer) override;

        grpc::Status ReportInvoicing(grpc::ServerContext *context, const communication::InvoicingReport *request,
                                     communication::InvoicingResponse *response) override;

        grpc::Status
        BookOnMobilityProvider(grpc::ServerContext *context, const communication::pub::BookingRequest *request,
                               communication::BookingResponse *response) override;

        grpc::Status StartUsing(grpc::ServerContext *context, const communication::StartUsingRequest *request,
                                communication::StartUsingResponse *response) override;

        grpc::Status EndUsing(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                              communication::EndUsingResponse *response) override;

        grpc::Status
        ReportUsageTotal(grpc::ServerContext *context, const communication::UsageTotalReportRequest *request,
                         communication::UsageTotalReportResponse *response) override;

    private:
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const std::unique_ptr<MobilityProviderClientManager> m_clientManager{nullptr};

        [[nodiscard]] std::uint64_t
        GetRequestTotalAndInsertSeat(const communication::pub::BookingRequest &request,
                                     google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap);
    };

}// yakbas
