#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "SecretCommunication.grpc.pb.h"
#include "CustomSealOperations.h"
#include "PlatformClientManager.h"

namespace yakbas::sec {

    class PlatformServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        using secretService = communication::sec::SecretCommunicationService;

        explicit PlatformServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status
        SearchForSecretRides(grpc::ServerContext *context, const communication::sec::SearchRequest *request,
                             grpc::ServerWriter<communication::sec::Journey> *writer) override;

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<communication::Journey> *writer) override;

        grpc::Status
        BookOnPlatform(grpc::ServerContext *context, grpc::ServerReader<communication::sec::BookingRequest> *reader,
                       communication::sec::BookingResponse *response) override;

        grpc::Status
        BookAsymmetricOnPlatform(grpc::ServerContext *context, const communication::sec::SearchRequest *request,
                                 communication::sec::BookingResponse *response) override;

        grpc::Status
        BookOnMobilityProviders(grpc::ServerContext *context,
                                grpc::ServerReader<communication::sec::BookingRequest> *reader,
                                communication::sec::BookingResponse *response) override;

        grpc::Status
        StartUsing(grpc::ServerContext *context, const communication::StartUsingRequest *request,
                                communication::sec::StartUsingResponse *response) override;

        grpc::Status
        EndUsing(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                              communication::sec::EndUsingResponse *response) override;

        grpc::Status
        StartUsingSymmetric(grpc::ServerContext *context, const communication::StartUsingRequest *request,
                            communication::sec::StartUsingResponse *response) override;

        grpc::Status
        EndUsingSymmetric(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                                       communication::sec::EndUsingResponse *response) override;

        grpc::Status
        ReportUsageTotal(grpc::ServerContext *context, const communication::sec::UsageTotalReportRequest *request,
                         communication::UsageTotalReportResponse *response) override;

        grpc::Status ReportInvoicing(grpc::ServerContext *context, const communication::InvoicingReport *request,
                                     communication::InvoicingResponse *response) override;

    private:
        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetRequestTotalAndInsertSeat(const communication::sec::BookingRequest &request,
                                     google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        FindSecretRideTotal(const communication::sec::Ride &ride, const seal::RelinKeys &relinKeys) const;

        void
        handleIsReadable(const std::unique_ptr<secretService::Stub> &stub_1,
                         const std::unique_ptr<secretService::Stub> &stub_2,
                         std::vector<std::unique_ptr<seal::Ciphertext>> &requestTotalCiphers, int count,
                         const std::unique_ptr<communication::sec::BookingRequest> &bookingRequestPtr) const;

        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const std::unique_ptr<PlatformClientManager> m_platformClientManager{nullptr};
        const seal::scheme_type m_schemeType{};
    };

}// yakbas
