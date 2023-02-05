
#include <log4cplus/loggingmacros.h>
#include "MobilityProviderServiceImpl.h"
#include "MobilityProviderGenerator.h"
#include "Timer.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    std::string FindLoggerName(const std::string &loggerInstance) {
        const char &i = loggerInstance.at(loggerInstance.size() - 1);
        return "Secret Mobility Provider Client Manager " + std::to_string(i);
    }

    MobilityProviderServiceImpl::MobilityProviderServiceImpl(const std::string &&loggerInstance,
                                                             const SealKeys &sealKeys)
            : m_customSealOperationsPtr(std::make_unique<CustomSealOperations>(sealKeys)),
              m_clientManager(std::make_unique<MobilityProviderClientManager>(FindLoggerName(loggerInstance))),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance(loggerInstance))),
              m_schemeType(sealKeys.m_schemeType) {}

    grpc::Status MobilityProviderServiceImpl::SearchForSecretRides(grpc::ServerContext *context,
                                                                   const communication::sec::SearchRequest *request,
                                                                   grpc::ServerWriter<communication::sec::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Secret Mobility Provider Service impl SearchForRides invoked...");

        const auto stream = GetUniqueStream(request->publickey());
        const auto publicKeyPtr = m_customSealOperationsPtr->GetPublicKeyFromBuffer(stream);
        const auto &operations = m_customSealOperationsPtr->GetSealOperations();
        const auto newEncryptorPtr = CustomSealOperations::CreateNewEncryptor(*publicKeyPtr,
                                                                              operations->GetSealInfoPtr()->m_sealKeys);

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

    grpc::Status MobilityProviderServiceImpl::StartUsing(grpc::ServerContext *context,
                                                         const communication::StartUsingRequest *request,
                                                         communication::sec::StartUsingResponse *response) {

        grpc::ClientContext clientContext;
        const auto status = m_clientManager->GetStub(constants::TRANSPORT_CHANNEL)->StartUsing(&clientContext, *request,
                                                                                               response);
        if (!status.ok()) {
            response->set_status(communication::FAILED);
            throw std::runtime_error("Start Using Request failed in " + m_logger->getName());
        }

        const auto &bufferPtr = m_customSealOperationsPtr->GetEncryptedBuffer(
                Timer::GetCurrentTimeMinutes());

        response->set_status(communication::SUCCESSFUL);
        response->set_start_time_in_minutes(*bufferPtr);
        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::StartUsingSymmetric(grpc::ServerContext *context,
                                                                  const communication::StartUsingRequest *request,
                                                                  communication::sec::StartUsingResponse *response) {
        grpc::ClientContext clientContext;
        const auto status = m_clientManager->GetStub(constants::TRANSPORT_CHANNEL)->StartUsing(&clientContext, *request,
                                                                                               response);
        if (!status.ok()) {
            response->set_status(communication::FAILED);
            throw std::runtime_error("Start Using Symmetric Request failed in " + m_logger->getName());
        }

        const auto &bufferPtr = m_customSealOperationsPtr->GetSymmetricEncryptedBuffer(
                Timer::GetCurrentTimeMinutes());

        response->set_status(communication::SUCCESSFUL);
        response->set_start_time_in_minutes(*bufferPtr);
        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::EndUsing(grpc::ServerContext *context,
                                                       const communication::EndUsingRequest *request,
                                                       communication::sec::EndUsingResponse *response) {
        grpc::ClientContext clientContext;
        const auto status = m_clientManager->GetStub(constants::TRANSPORT_CHANNEL)->EndUsing(&clientContext, *request,
                                                                                             response);
        if (!status.ok()) {
            throw std::runtime_error("End Using Request failed in " + m_logger->getName());
        }

        const auto &currentTimeBufferPtr = m_customSealOperationsPtr->GetEncryptedBuffer(
                Timer::GetCurrentTimeMinutes() + util::GetRandomNumber<int>());

        const auto &unitPriceBufferPtr = m_customSealOperationsPtr->GetEncryptedBuffer(
                util::GetRandomNumberVariant<std::uint64_t>()
        );

        response->set_end_time_in_minutes(*currentTimeBufferPtr);
        response->set_unit_price(*unitPriceBufferPtr);

        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::EndUsingSymmetric(grpc::ServerContext *context,
                                                                const communication::EndUsingRequest *request,
                                                                communication::sec::EndUsingResponse *response) {
        grpc::ClientContext clientContext;
        const auto status = m_clientManager->GetStub(constants::TRANSPORT_CHANNEL)->EndUsing(&clientContext, *request,
                                                                                             response);
        if (!status.ok()) {
            throw std::runtime_error("End Using Symmetric Request failed in " + m_logger->getName());
        }

        const auto &currentTimeBufferPtr = m_customSealOperationsPtr->GetSymmetricEncryptedBuffer(
                Timer::GetCurrentTimeMinutes() + util::GetRandomNumber<int>());

        const auto &unitPriceBufferPtr = m_customSealOperationsPtr->GetSymmetricEncryptedBuffer(
                util::GetRandomNumberVariant<std::uint64_t>()
        );

        response->set_end_time_in_minutes(*currentTimeBufferPtr);
        response->set_unit_price(*unitPriceBufferPtr);

        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::ReportUsageTotal(grpc::ServerContext *context,
                                                               const communication::sec::UsageTotalReportRequest *request,
                                                               communication::UsageTotalReportResponse *response) {
        try {
            const num_variant &variant = m_customSealOperationsPtr->DecryptFromBuffer(
                    GetUniqueStream(request->total()));
            LOG4CPLUS_TRACE(*m_logger, std::string("Decrypted report usage total: ") +
                                      std::to_string(GetAnyVariant<double>(&variant)));
        } catch (std::exception &e) {
            LOG4CPLUS_ERROR(*m_logger, std::string("Exception during decryption. Message: ") + e.what());
            throw std::runtime_error(e.what());
        }

        return grpc::Status::OK;
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