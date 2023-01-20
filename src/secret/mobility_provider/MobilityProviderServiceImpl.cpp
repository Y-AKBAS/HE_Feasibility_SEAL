
#include <log4cplus/loggingmacros.h>
#include "MobilityProviderServiceImpl.h"
#include "MobilityProviderGenerator.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    MobilityProviderServiceImpl::MobilityProviderServiceImpl(const std::string &&loggerInstance,
                                                             const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance(loggerInstance))),
              m_schemeType(sealKeys.m_schemeType) {}

    grpc::Status MobilityProviderServiceImpl::SearchForSecretRides(grpc::ServerContext *context,
                                                                   const communication::sec::SearchRequest *request,
                                                                   grpc::ServerWriter<communication::sec::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Mobility Provider Service impl SearchForRides invoked...");

        const auto stream = GetUniqueStream(request->publickey());
        const auto publicKeyPtr = m_customSealOperationsPtr->GetPublicKeyFromBuffer(stream);
        const auto newEncryptorPtr = CustomSealOperations::CreateNewEncryptor(*publicKeyPtr);

        const auto &operations = m_customSealOperationsPtr->GetSealOperations();

        auto status = MobilityProviderGenerator::GenerateSecretJourneys(request, writer, *operations, *newEncryptorPtr);
        return status;
    }

    grpc::Status MobilityProviderServiceImpl::SearchForRides(grpc::ServerContext *context,
                                                             const communication::SearchRequest *request,
                                                             grpc::ServerWriter<communication::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Mobility Provider Service impl SearchForRides invoked...");
        const auto &operations = m_customSealOperationsPtr->GetSealOperations();
        return MobilityProviderGenerator::GenerateJourneys(request, writer, *operations);
    }

    grpc::Status MobilityProviderServiceImpl::ReportInvoicing(grpc::ServerContext *context,
                                                              const communication::InvoicingReport *request,
                                                              communication::InvoicingResponse *response) {

        // Normally the other information should be written to the db :)
        response->set_status(communication::StatusCode::SUCCESSFUL);
        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::BookOnMobilityProvider(grpc::ServerContext *context,
                                                                     const communication::sec::BookingRequest *request,
                                                                     communication::sec::BookingResponse *response) {

        auto rideIdSeatNumberMap = response->mutable_rideidseatnumbermap();
        response->set_invoicingclerktype(request->invoicingclerktype());
        response->set_bookingtype(request->bookingtype());

        if (auto requestTotalPtr = GetRequestTotalAndInsertSeat(*request, rideIdSeatNumberMap)) {
            const auto &buffer = CustomSealOperations::GetBufferFromCipher(*requestTotalPtr);
            response->set_total(buffer);
            return grpc::Status::OK;
        }

        return {grpc::StatusCode::INTERNAL, m_logger->getName() + " Ciphertext computation failed..."};
    }

    std::unique_ptr<seal::Ciphertext>
    MobilityProviderServiceImpl::GetRequestTotalAndInsertSeat(const communication::sec::BookingRequest &request,
                                                              google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap) const {
        try {
            const auto &relinKeys = request.relinkeys();
            const auto relinKeysPtr = m_customSealOperationsPtr->GetRelinKeysFromBuffer(GetUniqueStream(relinKeys));

            const auto &unitPrice = request.unitprice();
            auto unitPriceCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(unitPrice));

            const auto &coefficient = request.coefficient();
            auto coefficientCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(coefficient));

            auto totalCipherPtr = m_customSealOperationsPtr->GetSealOperations()->GetNewCipher(
                    std::make_optional(coefficientCipherPtr->parms_id()));

            const auto &evaluatorPtr = m_customSealOperationsPtr->GetEvaluatorPtr();
            evaluatorPtr->multiply(*unitPriceCipherPtr, *coefficientCipherPtr,
                                   *totalCipherPtr);

            m_customSealOperationsPtr->GetSealOperations()->Relinearize(*totalCipherPtr, *relinKeysPtr);

            const auto &discount = request.discount();
            if (!discount.empty()) {
                auto discountCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(discount));
                m_customSealOperationsPtr->SubProcessedInPlace(*totalCipherPtr, *discountCipherPtr);
            }

            const auto &seatPrice = request.seatprice();
            if (!seatPrice.empty()) {
                auto seatPriceCipherPtr = m_customSealOperationsPtr->GetCipherFromBuffer(GetUniqueStream(seatPrice));
                m_customSealOperationsPtr->AddProcessedInPlace(*totalCipherPtr, *seatPriceCipherPtr);
                rideIdSeatNumberMap->emplace(request.ride_id(), request.seatnumber());
            }

            if (m_schemeType != seal::scheme_type::ckks) {
                m_customSealOperationsPtr->SwitchMode(*totalCipherPtr);
            }

            return totalCipherPtr;
        } catch (const std::exception &exception) {
            LOG4CPLUS_ERROR(*m_logger,
                            std::string("Error occurred while getting request total. Message: ") + exception.what());
            return nullptr;
        }
    }

} // yakbas