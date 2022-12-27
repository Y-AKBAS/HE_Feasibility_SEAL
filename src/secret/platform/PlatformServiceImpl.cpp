
#include <log4cplus/loggingmacros.h>

#include <utility>
#include "PlatformServiceImpl.h"
#include "ApplicationConstants.h"
#include "Utils.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    PlatformServiceImpl::PlatformServiceImpl(const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_platformClientManager(std::make_unique<PlatformClientManager>()),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Platform Service Impl"))) {}

    grpc::Status PlatformServiceImpl::SearchForSecretRides(grpc::ServerContext *context,
                                                           const communication::sec::SearchRequest *request,
                                                           grpc::ServerWriter<communication::sec::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Service impl SearchForSecretRides invoked...");

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL);
        grpc::ClientContext clientContext;
        const auto clientReaderPtr = stubPtr->SearchForSecretRides(&clientContext, *request);

        bool isReadable;
        do {
            const auto journeyPtr = GetUnique<communication::sec::Journey>();
            isReadable = clientReaderPtr->Read(journeyPtr.get());
            if (isReadable) {
                writer->Write(*journeyPtr);
            }
        } while (isReadable);

        const grpc::Status &status = clientReaderPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Journeys sent successfully ... ");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during SearchForSecretRides(). Error message: " + status.error_message());
        }

        return status;
    }

    grpc::Status PlatformServiceImpl::SearchForRides(grpc::ServerContext *context,
                                                     const communication::SearchRequest *request,
                                                     grpc::ServerWriter<communication::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Service impl SearchForRides invoked...");

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL);
        grpc::ClientContext clientContext;
        const auto clientReaderPtr = stubPtr->SearchForRides(&clientContext, *request);

        bool isReadable;
        do {
            const auto journeyPtr = GetUnique<communication::Journey>();
            isReadable = clientReaderPtr->Read(journeyPtr.get());
            if (isReadable) {
                writer->Write(*journeyPtr);
            }
        } while (isReadable);

        const grpc::Status &status = clientReaderPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Journeys sent successfully ... ");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during SearchForRides(). Error message: " + status.error_message());
        }

        return status;
    }

    grpc::Status PlatformServiceImpl::Book(grpc::ServerContext *context,
                                           grpc::ServerReader<communication::sec::BookingRequest> *reader,
                                           communication::sec::BookingResponse *response) {

        response->set_journey_id(GetUUID());
        const auto &evaluatorPtr = m_customSealOperationsPtr->GetEvaluatorPtr();
        auto rideIdSeatNumberMap = response->mutable_rideidseatnumbermap();

        std::vector<std::unique_ptr<seal::Ciphertext>> requestTotalCiphers{};
        bool isReadable;
        do {
            const auto bookingRequestPtr = GetUnique<communication::sec::BookingRequest>();
            isReadable = reader->Read(bookingRequestPtr.get());
            if (isReadable) {
                const static bool isSet = [&bookingRequestPtr, &response]() -> bool {
                    response->set_invoicingclerktype(bookingRequestPtr->invoicingclerktype());
                    response->set_bookingtype(bookingRequestPtr->bookingtype());
                    return true;
                }();
                auto requestTotalPtr = GetRequestTotalAndInsertSeat(*bookingRequestPtr, rideIdSeatNumberMap);
                requestTotalCiphers.push_back(std::move(requestTotalPtr));
            }
        } while (isReadable);

        if (requestTotalCiphers.size() > 1) {
            for (int i = 1; i < requestTotalCiphers.size(); ++i) {
                auto newCipherPtr = GetUnique<seal::Ciphertext>();
                evaluatorPtr->add(*requestTotalCiphers.at(0), *requestTotalCiphers.at(i), *newCipherPtr);
                requestTotalCiphers.at(0) = std::move(newCipherPtr);
            }
        }

        std::unique_ptr<seal::Ciphertext> &totalCipherPtr = requestTotalCiphers.at(0);
        m_customSealOperationsPtr->SwitchMode(*totalCipherPtr);
        const auto buffer = CustomSealOperations::GetBufferFromCipher(*totalCipherPtr);
        response->set_total(buffer);

        return grpc::Status::OK;
    }

    grpc::Status PlatformServiceImpl::ReportInvoicing(grpc::ServerContext *context,
                                                      const communication::InvoicingReport *request,
                                                      communication::InvoicingResponse *response) {

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL);
        grpc::ClientContext clientContext;
        const auto &status = stubPtr->ReportInvoicing(&clientContext, *request, response);

        if (!status.ok()) {
            throw std::runtime_error("Reporting secret invoice failed in Secret Platform");
        }

        response->set_status(communication::StatusCode::SUCCESSFUL);
        return status;
    }

    std::unique_ptr<seal::Ciphertext>
    PlatformServiceImpl::GetRequestTotalAndInsertSeat(const communication::sec::BookingRequest &request,
                                                      google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap) const {

        const auto &unitPrice = request.unitprice();
        auto unitPriceCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(unitPrice));

        const auto &coefficient = request.coefficient();
        auto coefficientCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(coefficient));

        auto totalCipherPtr = GetUnique<seal::Ciphertext>();
        const auto &evaluatorPtr = m_customSealOperationsPtr->GetEvaluatorPtr();
        evaluatorPtr->multiply(*unitPriceCipherPtr, *coefficientCipherPtr,
                               *totalCipherPtr);

        const auto &discount = request.discount();
        if (!discount.empty()) {
            auto newCipherPtr = GetUnique<seal::Ciphertext>();
            auto discountCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(discount));
            evaluatorPtr->sub(*totalCipherPtr, *discountCipherPtr, *newCipherPtr);
            totalCipherPtr = std::move(newCipherPtr);
        }

        const auto &seatPrice = request.seatprice();
        if (!seatPrice.empty()) {
            auto newCipherPtr = GetUnique<seal::Ciphertext>();
            auto seatPriceCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(seatPrice));
            evaluatorPtr->add(*totalCipherPtr, *seatPriceCipherPtr, *newCipherPtr);
            totalCipherPtr = std::move(newCipherPtr);
            rideIdSeatNumberMap->emplace(request.ride_id(), request.seatnumber());
        }

        m_customSealOperationsPtr->SwitchMode(*totalCipherPtr);

        return totalCipherPtr;
    }

} // yakbas