#pragma once

#include <grpcpp/grpcpp.h>
#include <log4cplus/logger.h>
#include "SecretCommunication.grpc.pb.h"
#include "CustomSealOperations.h"
#include "PlatformClientManager.h"

namespace yakbas::sec {

    class PlatformServiceImpl final : public communication::sec::SecretCommunicationService::Service {

    public:
        explicit PlatformServiceImpl(const SealKeys &sealKeys = {});

        grpc::Status
        SearchForSecretRides(grpc::ServerContext *context, const communication::sec::SearchRequest *request,
                             grpc::ServerWriter<communication::sec::Journey> *writer) override;

        grpc::Status SearchForRides(grpc::ServerContext *context, const communication::SearchRequest *request,
                                    grpc::ServerWriter<communication::Journey> *writer) override;

        grpc::Status
        Book(grpc::ServerContext *context, grpc::ServerReader<communication::sec::BookingRequest> *reader,
             communication::sec::BookingResponse *response) override;

        grpc::Status ReportInvoicing(grpc::ServerContext *context, const communication::InvoicingReport *request,
                                     communication::InvoicingResponse *response) override;

    private:
        const std::unique_ptr<CustomSealOperations> m_customSealOperationsPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const std::unique_ptr<PlatformClientManager> m_platformClientManager{nullptr};
        seal::scheme_type m_schemeType;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetRequestTotalAndInsertSeat(const communication::sec::BookingRequest &request,
                                     google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap) const;
    };

}// yakbas
