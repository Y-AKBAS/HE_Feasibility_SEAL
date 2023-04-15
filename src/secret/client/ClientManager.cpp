
#include <log4cplus/loggingmacros.h>
#include "ClientManager.h"
#include "ClientGenerator.h"
#include "SecretCommunication.pb.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    ClientManager::~ClientManager() = default;

    ClientManager::ClientManager(const SealKeys &sealKeys)
            : m_userPtr(ClientGenerator::GenerateSecretUser(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("SecretClientManager"))),
              m_transporterUsageMap{} {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Secret Client Manager is being initialized...");
            ClientManager::CreateChannels(
                    {{constants::INVOICE_CLERK_CHANNEL, SECRET_INVOICE_CLERK_SERVER_PORT},
                     {constants::PLATFORM_CHANNEL,      SECRET_PLATFORM_SERVER_PORT}}
            );
            this->GetPublicKey();
        });
        m_schemeType = m_userPtr->GetCustomSealOperations()->GetSealOperations()->GetSealInfoPtr()->m_sealKeys.m_schemeType;
    }

    std::map<std::string, const std::shared_ptr<seal::PublicKey>> ClientManager::m_publicKeyMap{};
    std::once_flag ClientManager::m_isInitialized{};

    void ClientManager::GetPublicKey() const {

        const std::unique_ptr<secretService::Stub> invoiceClerkStubPtr
                = this->GetStub(constants::INVOICE_CLERK_CHANNEL);

        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto request = GetUnique<google::protobuf::Empty>();
        const auto response = GetUnique<communication::sec::PublicKey>();

        invoiceClerkStubPtr->GetPublicKey(clientContext.get(), *request, response.get());

        auto invoiceClerkPublicKey = m_userPtr->GetCustomSealOperations()->GetPublicKeyFromBuffer(
                GetUniqueStream(response->publickey()));

        LOG4CPLUS_DEBUG(*m_logger, "Secret Client Manager requested public key...");

        std::shared_ptr<seal::PublicKey> key(invoiceClerkPublicKey.release());
        m_publicKeyMap.insert(std::make_pair(constants::INVOICE_CLERK_CHANNEL, key));
    }

    bool ClientManager::IsInitialized() {
        return !m_publicKeyMap.empty() && !m_channelMap.empty();
    }

    seal::scheme_type ClientManager::GetSchemeType() const {
        return m_schemeType;
    }

    std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
    ClientManager::SearchSecretly(const std::string &from, const std::string &to, int numberOfJourneys) const {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);

        auto journeyVecPtr = GetUnique<std::vector<std::unique_ptr<communication::Journey>>>();

        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto request = GetUnique<communication::sec::SearchRequest>();

        request->set_numberofjourneys(numberOfJourneys);
        request->set_to(to);
        request->set_from(from);
        request->set_publickey(m_userPtr->GetCustomSealOperations()->GetPublicKeyBuffer());

        const auto readerPtr = stubPtr->SearchForSecretRides(clientContext.get(), *request);

        bool isReadable;
        do {
            auto journeyPtr = GetUnique<communication::sec::Journey>();
            isReadable = readerPtr->Read(journeyPtr.get());

            if (isReadable) {
                auto publicJourneyPtr = MapSecretToPublic(*journeyPtr);
                journeyVecPtr->push_back(std::move(publicJourneyPtr));
            }
        } while (isReadable);

        const grpc::Status &status = readerPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_DEBUG(*m_logger, "Fetched Secret Journeys successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during SearchSecretly. Error message: " + status.error_message());
        }

        return journeyVecPtr;
    }

    std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
    ClientManager::Search(const std::string &from, const std::string &to, int numberOfJourneys) const {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);

        auto journeyVecPtr =
                GetUnique<std::vector<std::unique_ptr<communication::Journey>>>();

        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto request = GetUnique<communication::SearchRequest>();

        request->set_numberofjourneys(numberOfJourneys);
        request->set_to(to);
        request->set_from(from);

        const auto readerPtr = stubPtr->SearchForRides(clientContext.get(), *request);

        bool isReadable;
        do {
            auto journeyPtr = GetUnique<communication::Journey>();
            isReadable = readerPtr->Read(journeyPtr.get());

            if (isReadable) {
                journeyVecPtr->push_back(std::move(journeyPtr));
            }
        } while (isReadable);

        const grpc::Status &status = readerPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_DEBUG(*m_logger, "Fetched Journeys successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Search. Error message: " + status.error_message());
        }

        return journeyVecPtr;
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookOnPlatformAndDecrypt(const communication::Journey &journey) const {
        const auto secretBookingResponsePtr = this->BookOnPlatform(journey);
        return this->MapSecretToPublic(*secretBookingResponsePtr);
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookSymmetricOnPlatformAndDecrypt(const communication::Journey &journey) const {
        const auto secretBookingResponsePtr = this->BookSymmetricOnPlatform(journey);
        return this->MapSecretToPublic(*secretBookingResponsePtr);
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookSymmetricOnMobilityProvidersAndDecrypt(const communication::Journey &journey) const {
        const auto secretBookingResponsePtr = this->BookSymmetricOnMobilityProviders(journey);
        return this->MapSecretToPublic(*secretBookingResponsePtr);
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookOnMobilityProvidersAndDecrypt(const communication::Journey &journey) const {
        const auto secretBookingResponsePtr = this->BookOnMobilityProviders(journey);
        return this->MapSecretToPublic(*secretBookingResponsePtr);
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookAsymmetricOnPlatformAndDecrypt(const std::string &from, const std::string &to,
                                                      int numberOfJourneys) const {
        const auto secretBookingResponsePtr = BookAsymmetricOnPlatform(from, to, numberOfJourneys);
        return this->MapSecretToPublic(*secretBookingResponsePtr);
    }

    std::unique_ptr<communication::sec::BookingResponse>
    ClientManager::BookOnPlatform(const communication::Journey &journey) const {

        const bool isCKKS = m_schemeType == seal::scheme_type::ckks;

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::BookingResponse>();
        const auto clientWriterPtr = stubPtr->BookOnPlatform(clientContextPtr.get(), responsePtr.get());

        const auto &rides = journey.rides();

        for (const auto &ride: rides) {

            const auto requestPtr = GetUnique<communication::sec::BookingRequest>();
            const auto &customSealOperations = m_userPtr->GetCustomSealOperations();
            requestPtr->set_relinkeys(customSealOperations->GetRelinKeysBuffer());

            int bookingType = GetRandomNumber<int>() % (communication::BookingType_ARRAYSIZE - 1);
            requestPtr->set_bookingtype(static_cast<communication::BookingType>(bookingType));

            const auto &coefficientVariant = AnyToNumVariant(isCKKS, &ride.coefficient());
            const auto coefficientBuffer = customSealOperations->GetEncryptedBuffer(coefficientVariant);
            requestPtr->set_coefficient(*coefficientBuffer);

            const auto &unitPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().unitprice());
            const auto unitPriceBuffer = customSealOperations->GetEncryptedBuffer(unitPriceVariant);
            requestPtr->set_unitprice(*unitPriceBuffer);

            const auto &discountVariant = AnyToNumVariant(isCKKS, &ride.discount());
            if (GetAnyVariant<double>(&discountVariant) > 0) {
                const auto discountBuffer = customSealOperations->GetEncryptedBuffer(discountVariant);
                requestPtr->set_discount(*discountBuffer);
            }

            const auto &seatPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().seatprice());
            if (GetAnyVariant<double>(&seatPriceVariant) > 0) {
                const auto seatPriceBuffer = customSealOperations->GetEncryptedBuffer(seatPriceVariant);
                requestPtr->set_seatprice(*seatPriceBuffer);
            }

            if (!clientWriterPtr->Write(*requestPtr)) {
                throw std::bad_function_call();
            }
        }

        clientWriterPtr->WritesDone();
        const auto &status = clientWriterPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_TRACE(*m_logger, "Secret BookOnPlatform was a success...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Secret BookOnPlatform. Error message: " +
                            status.error_message());
        }

        return responsePtr;
    }

    std::unique_ptr<communication::sec::BookingResponse>
    ClientManager::BookSymmetricOnPlatform(const communication::Journey &journey) const {

        const bool isCKKS = m_schemeType == seal::scheme_type::ckks;

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::BookingResponse>();
        const auto clientWriterPtr = stubPtr->BookOnPlatform(clientContextPtr.get(), responsePtr.get());

        const auto &rides = journey.rides();

        for (const auto &ride: rides) {

            const auto requestPtr = GetUnique<communication::sec::BookingRequest>();
            const auto &customSealOperations = m_userPtr->GetCustomSealOperations();
            requestPtr->set_relinkeys(customSealOperations->GetRelinKeysBuffer());

            int bookingType = (GetRandomNumber<int>()) % (communication::BookingType_ARRAYSIZE - 1);
            requestPtr->set_bookingtype(static_cast<communication::BookingType>(bookingType));

            const auto &coefficientVariant = AnyToNumVariant(isCKKS, &ride.coefficient());
            const auto coefficientBuffer = customSealOperations->GetSymmetricEncryptedBuffer(coefficientVariant);
            requestPtr->set_coefficient(*coefficientBuffer);

            const auto &unitPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().unitprice());
            const auto unitPriceBuffer = customSealOperations->GetSymmetricEncryptedBuffer(unitPriceVariant);
            requestPtr->set_unitprice(*unitPriceBuffer);

            const auto &discountVariant = AnyToNumVariant(isCKKS, &ride.discount());
            if (GetAnyVariant<double>(&discountVariant) > 0) {
                const auto discountBuffer = customSealOperations->GetSymmetricEncryptedBuffer(discountVariant);
                requestPtr->set_discount(*discountBuffer);
            }

            const auto &seatPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().seatprice());
            if (GetAnyVariant<double>(&seatPriceVariant) > 0) {
                const auto seatPriceBuffer = customSealOperations->GetSymmetricEncryptedBuffer(seatPriceVariant);
                requestPtr->set_seatprice(*seatPriceBuffer);
            }

            if (!clientWriterPtr->Write(*requestPtr)) {
                throw std::bad_function_call();
            }
        }

        clientWriterPtr->WritesDone();
        const auto &status = clientWriterPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_TRACE(*m_logger, "Sent Symmetric Secret BookingRequests successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Sending Symmetric Secret BookingRequests. Error message: " +
                            status.error_message());
        }

        return responsePtr;
    }

    std::unique_ptr<communication::sec::BookingResponse>
    ClientManager::BookSymmetricOnMobilityProviders(const communication::Journey &journey) const {
        const bool isCKKS = m_schemeType == seal::scheme_type::ckks;

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::BookingResponse>();
        const auto clientWriterPtr = stubPtr->BookOnMobilityProviders(clientContextPtr.get(), responsePtr.get());

        for (const auto &ride: journey.rides()) {
            const auto requestPtr = MapJourneyToBookingRequest(ride, isCKKS);
            if (!clientWriterPtr->Write(*requestPtr)) {
                throw std::bad_function_call();
            }
        }

        clientWriterPtr->WritesDone();
        const auto &status = clientWriterPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_TRACE(*m_logger, "Secret BookSymmetricOnMobilityProviders was a success...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Secret BookSymmetricOnMobilityProviders. Error message: " +
                            status.error_message());
        }

        return responsePtr;
    }

    std::unique_ptr<communication::sec::BookingResponse>
    ClientManager::BookAsymmetricOnPlatform(const std::string &from, const std::string &to,
                                            int numberOfJourneys) const {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContext = GetUnique<grpc::ClientContext>();
        const auto requestPtr = GetUnique<communication::sec::SearchRequest>();

        requestPtr->set_numberofjourneys(numberOfJourneys);
        requestPtr->set_to(to);
        requestPtr->set_from(from);
        requestPtr->set_publickey(m_userPtr->GetCustomSealOperations()->GetPublicKeyBuffer());
        requestPtr->set_relinkeys(m_userPtr->GetCustomSealOperations()->GetRelinKeysBuffer());

        m_logger->log(log4cplus::INFO_LOG_LEVEL, "ClientManager::BookAsymmetricOnPlatform request_size: " +
                                                 std::to_string(requestPtr->ByteSizeLong()));

        auto responsePtr = GetUnique<communication::sec::BookingResponse>();
        const auto &status = stubPtr->BookAsymmetricOnPlatform(
                clientContext.get(), *requestPtr, responsePtr.get());

        m_logger->log(log4cplus::INFO_LOG_LEVEL, "ClientManager::BookAsymmetricOnPlatform response_size: " +
                                                 std::to_string(responsePtr->ByteSizeLong()));
        if (status.ok()) {
            LOG4CPLUS_TRACE(*m_logger, "Secret BookAsymmetricOnPlatform was a success...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during BookAsymmetricOnPlatform. Error message: " +
                            status.error_message());
        }

        return responsePtr;
    }

    std::unique_ptr<communication::sec::BookingResponse>
    ClientManager::BookOnMobilityProviders(const communication::Journey &journey) const {
        const bool isCKKS = m_schemeType == seal::scheme_type::ckks;

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::BookingResponse>();
        const auto clientWriterPtr = stubPtr->BookOnMobilityProviders(clientContextPtr.get(), responsePtr.get());

        const auto &rides = journey.rides();
        std::uint64_t chunkCount{};

        const auto requestPtr = GetUnique<communication::sec::BookingRequest>();
        const auto &customSealOperations = m_userPtr->GetCustomSealOperations();
        requestPtr->set_relinkeys(customSealOperations->GetRelinKeysBuffer());

        m_logger->log(log4cplus::INFO_LOG_LEVEL,
                      "relinkeys size: " + std::to_string(customSealOperations->GetRelinKeysBuffer().size()));
        m_logger->log(log4cplus::INFO_LOG_LEVEL,
                      "public key size: " + std::to_string(customSealOperations->GetPublicKeyBuffer().size()));
        m_logger->log(log4cplus::INFO_LOG_LEVEL,
                      "ClientManager::BookOnMobilityProviders request with relinkeys: " +
                      std::to_string(requestPtr->ByteSizeLong()));

        for (const auto &ride: rides) {
            chunkCount++;
            int bookingType = GetRandomNumber<int>() % (communication::BookingType_ARRAYSIZE - 1);
            requestPtr->set_bookingtype(static_cast<communication::BookingType>(bookingType));

            const auto &coefficientVariant = AnyToNumVariant(isCKKS, &ride.coefficient());
            const auto coefficientBuffer = customSealOperations->GetEncryptedBuffer(coefficientVariant);
            requestPtr->set_coefficient(*coefficientBuffer);

            const auto &unitPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().unitprice());
            const auto unitPriceBuffer = customSealOperations->GetEncryptedBuffer(unitPriceVariant);
            requestPtr->set_unitprice(*unitPriceBuffer);

            const auto &discountVariant = AnyToNumVariant(isCKKS, &ride.discount());
            if (GetAnyVariant<double>(&discountVariant) > 0) {
                const auto discountBuffer = customSealOperations->GetEncryptedBuffer(discountVariant);
                requestPtr->set_discount(*discountBuffer);
            }

            const auto &seatPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().seatprice());
            if (GetAnyVariant<double>(&seatPriceVariant) > 0) {
                const auto seatPriceBuffer = customSealOperations->GetEncryptedBuffer(seatPriceVariant);
                requestPtr->set_seatprice(*seatPriceBuffer);
            }

            m_logger->log(log4cplus::INFO_LOG_LEVEL,
                          "ClientManager::BookOnMobilityProviders " + std::to_string(chunkCount) +
                          ". request chunk: " + std::to_string(requestPtr->ByteSizeLong()));

            if (!clientWriterPtr->Write(*requestPtr)) {
                throw std::bad_function_call();
            }
        }

        clientWriterPtr->WritesDone();
        const auto &status = clientWriterPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_TRACE(*m_logger, "Secret BookOnMobilityProviders was a success...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred BookOnMobilityProviders. Error message: " +
                            status.error_message());
        }

        m_logger->log(log4cplus::INFO_LOG_LEVEL,
                      "ClientManager::BookOnMobilityProviders response size:" +
                      std::to_string(responsePtr->ByteSizeLong()));

        return responsePtr;
    }

    void ClientManager::SendStartUsingRequest() {

        const std::string transporterId = util::GetUUID();
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::StartUsingResponse>();
        auto requestPtr = GetUnique<communication::StartUsingRequest>();
        requestPtr->set_user_id(this->m_userPtr->GetId());
        requestPtr->set_transporter_id(transporterId);

        const auto &status = this->GetStub(constants::PLATFORM_CHANNEL)->StartUsing(clientContextPtr.get(),
                                                                                    *requestPtr, responsePtr.get());
        if (!status.ok()) {
            const auto errorMessage = "Failed sending start using request";
            LOG4CPLUS_ERROR(*m_logger, errorMessage);
            throw std::runtime_error(errorMessage);
        }

        auto cipherPtr = m_userPtr->GetCustomSealOperations()->GetCipherFromBuffer(
                GetUniqueStream(responsePtr->start_time_in_minutes()));
        m_transporterUsageMap.insert({transporterId, std::move(cipherPtr)});
    }

    void ClientManager::SendStartUsingRequestSymmetric() {

        const std::string transporterId = util::GetUUID();
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::StartUsingResponse>();
        auto requestPtr = GetUnique<communication::StartUsingRequest>();
        requestPtr->set_user_id(this->m_userPtr->GetId());
        requestPtr->set_transporter_id(transporterId);

        const auto &status = this->GetStub(constants::PLATFORM_CHANNEL)->StartUsingSymmetric(clientContextPtr.get(),
                                                                                             *requestPtr,
                                                                                             responsePtr.get());
        if (!status.ok()) {
            const auto errorMessage = "Failed sending start using symmetric request";
            LOG4CPLUS_ERROR(*m_logger, errorMessage);
            throw std::runtime_error(errorMessage);
        }

        auto cipherPtr = m_userPtr->GetCustomSealOperations()->GetCipherFromBuffer(
                GetUniqueStream(responsePtr->start_time_in_minutes()));
        m_transporterUsageMap.insert({transporterId, std::move(cipherPtr)});
    }

    void ClientManager::SendEndUsingRequest() {
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::EndUsingResponse>();
        const auto requestPtr = GetUnique<communication::EndUsingRequest>();

        requestPtr->set_user_id(m_userPtr->GetId());
        requestPtr->set_transporter_id(m_transporterUsageMap.begin()->first);

        const auto status = this->GetStub(constants::PLATFORM_CHANNEL)->EndUsing(clientContextPtr.get(), *requestPtr,
                                                                                 responsePtr.get());
        if (!status.ok()) {
            LOG4CPLUS_ERROR(*m_logger, "End using symmetric request failed...");
            throw std::runtime_error(status.error_message());
        }
        const auto &evaluatorPtr = m_userPtr->GetCustomSealOperations()->GetEvaluatorPtr();

        auto endTimeInMins = m_userPtr->GetCustomSealOperations()->GetCipherFromBuffer(
                GetUniqueStream(responsePtr->end_time_in_minutes()));
        auto unitPrice = m_userPtr->GetCustomSealOperations()->GetCipherFromBuffer(
                GetUniqueStream(responsePtr->unit_price()));

        evaluatorPtr->sub_inplace(*endTimeInMins, *m_transporterUsageMap.begin()->second);
        evaluatorPtr->multiply_inplace(*endTimeInMins, *unitPrice);

        this->SendUsageTotal(this->m_userPtr->GetCustomSealOperations()->GetBufferFromCipher(*endTimeInMins));
    }

    void ClientManager::SendEndUsingRequestSymmetric() {
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::sec::EndUsingResponse>();
        const auto requestPtr = GetUnique<communication::EndUsingRequest>();

        requestPtr->set_user_id(m_userPtr->GetId());
        requestPtr->set_transporter_id(m_transporterUsageMap.begin()->first);

        const auto status = this->GetStub(constants::PLATFORM_CHANNEL)->EndUsingSymmetric(clientContextPtr.get(),
                                                                                          *requestPtr,
                                                                                          responsePtr.get());
        if (!status.ok()) {
            LOG4CPLUS_ERROR(*m_logger, "End using symmetric request failed...");
            throw std::runtime_error(status.error_message());
        }
        const auto &evaluatorPtr = m_userPtr->GetCustomSealOperations()->GetEvaluatorPtr();

        auto endTimeInMins = m_userPtr->GetCustomSealOperations()->GetCipherFromBuffer(
                GetUniqueStream(responsePtr->end_time_in_minutes()));
        auto unitPrice = m_userPtr->GetCustomSealOperations()->GetCipherFromBuffer(
                GetUniqueStream(responsePtr->unit_price()));

        evaluatorPtr->sub_inplace(*endTimeInMins, *m_transporterUsageMap.begin()->second);
        evaluatorPtr->multiply_inplace(*endTimeInMins, *unitPrice);

        this->SendUsageTotal(this->m_userPtr->GetCustomSealOperations()->GetBufferFromCipher(*endTimeInMins));
    }

    void ClientManager::SendUsageTotal(const std::string &total) {

        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        const auto requestPtr = GetUnique<communication::sec::UsageTotalReportRequest>();
        const auto responsePtr = GetUnique<communication::UsageTotalReportResponse>();

        requestPtr->set_transporter_id(m_transporterUsageMap.begin()->first);
        requestPtr->set_user_id(this->m_userPtr->GetId());
        requestPtr->set_total(total);

        const auto &status = this->GetStub(constants::PLATFORM_CHANNEL)->ReportUsageTotal(clientContextPtr.get(),
                                                                                          *requestPtr,
                                                                                          responsePtr.get());
        if (!status.ok()) {
            LOG4CPLUS_ERROR(*m_logger, "Sending usage total failed");
            throw std::runtime_error(status.error_message());
        }

        m_transporterUsageMap.clear();
    }

    std::unique_ptr<communication::InvoicingResponse>
    ClientManager::Pay(const communication::BookingResponse &bookingResponse) const {

        const bool isCKKS = m_schemeType == seal::scheme_type::ckks;

        const auto stubPtr = this->GetStub(constants::INVOICE_CLERK_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto invoicingResponsePtr = GetUnique<communication::InvoicingResponse>();

        const auto invoicingRequestPtr = GetUnique<communication::InvoicingRequest>();
        const auto &variant = AnyToNumVariant(isCKKS, &bookingResponse.total());
        invoicingRequestPtr->set_price(GetAnyVariant<double>(&variant));
        const auto protoUserPtr = invoicingRequestPtr->mutable_user();
        m_userPtr->ToProto(protoUserPtr);

        const grpc::Status &status = stubPtr->CreateInvoice(clientContextPtr.get(), *invoicingRequestPtr,
                                                            invoicingResponsePtr.get());
        if (status.ok()) {
            LOG4CPLUS_DEBUG(*m_logger, "Sent InvoicingRequest successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during creating InvoicingRequest. Error message: " +
                            status.error_message());
        }

        return invoicingResponsePtr;
    }

    communication::StatusCode
    ClientManager::ReportInvoicing(const communication::InvoicingResponse &invoicingResponse,
                                   const communication::BookingResponse &bookingResponse) const {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto invoicingReport = GetUnique<communication::InvoicingReport>();

        invoicingReport->set_bookingtype(bookingResponse.bookingtype());
        invoicingReport->set_userid(m_userPtr->GetId());
        invoicingReport->set_journeyid(bookingResponse.journey_id());

        auto rideAndSeatNumberMapPtr = invoicingReport->mutable_rideidseatnumbermap();
        MapRideAndSeatNumberMap(*rideAndSeatNumberMapPtr, &bookingResponse.rideidseatnumbermap());

        auto newInvoicingResponsePtr = GetUnique<communication::InvoicingResponse>();
        stubPtr->ReportInvoicing(clientContextPtr.get(), *invoicingReport, newInvoicingResponsePtr.get());
        return newInvoicingResponsePtr->status();
    }

    std::unique_ptr<communication::Journey>
    ClientManager::MapSecretToPublic(const communication::sec::Journey &secretJourney) const {

        auto publicJourneyPtr = GetUnique<communication::Journey>();
        const auto &secretRides = secretJourney.rides();

        for (const auto &secretRide: secretRides) {

            communication::Ride *publicRidePtr = publicJourneyPtr->add_rides();

            publicRidePtr->set_from(secretRide.from());
            publicRidePtr->set_to(secretRide.to());
            publicRidePtr->set_providerid(secretRide.providerid());
            publicRidePtr->set_rideid(secretRide.rideid());

            const auto &cipherCoefficient = secretRide.coefficient();
            if (!cipherCoefficient.empty()) {
                const auto &coefficientVar = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(cipherCoefficient));
                NumVariantToAny(&coefficientVar, publicRidePtr->mutable_coefficient());
            } else {
                throw std::logic_error("Coefficient cannot be empty");
            }

            const auto &cipherDiscount = secretRide.discount();
            if (!cipherDiscount.empty()) {
                const auto &discountVar = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(cipherDiscount));
                NumVariantToAny(&discountVar, publicRidePtr->mutable_discount());
            }

            const auto timestampPtr = publicRidePtr->mutable_starttime();
            timestampPtr->set_seconds(secretRide.starttime().seconds());
            timestampPtr->set_nanos(secretRide.starttime().nanos());

            const auto transporterPtr = publicRidePtr->mutable_transporter();
            transporterPtr->set_providerid(secretRide.transporter().providerid());
            transporterPtr->set_unitpricetype(secretRide.transporter().unitpricetype());
            transporterPtr->set_transportertype(secretRide.transporter().transportertype());
            transporterPtr->set_capacity(secretRide.transporter().capacity());

            const auto &cipherUnitPrice = secretRide.transporter().unitprice();
            if (!cipherUnitPrice.empty()) {
                const auto &unitPriceVar = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(cipherUnitPrice));
                NumVariantToAny(&unitPriceVar, transporterPtr->mutable_unitprice());
            } else {
                throw std::logic_error("UnitPrice cannot be empty");
            }

            const auto &cipherSeatPrice = secretRide.transporter().seatprice();
            if (!cipherSeatPrice.empty()) {
                const auto &seatPriceVar = m_userPtr->GetCustomSealOperations()->DecryptFromBuffer(
                        GetUniqueStream(cipherSeatPrice));
                NumVariantToAny(&seatPriceVar, transporterPtr->mutable_seatprice());
            }
        }

        return publicJourneyPtr;
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::MapSecretToPublic(const communication::sec::BookingResponse &bookingResponse) const {

        auto publicResponsePtr = GetUnique<communication::BookingResponse>();

        publicResponsePtr->set_bookingtype(bookingResponse.bookingtype());
        publicResponsePtr->set_invoicingclerktype(bookingResponse.invoicingclerktype());
        publicResponsePtr->set_journey_id(bookingResponse.journey_id());

        auto rideIdSeatNumberMapPtr = publicResponsePtr->mutable_rideidseatnumbermap();
        for (const auto &pair: bookingResponse.rideidseatnumbermap()) {
            rideIdSeatNumberMapPtr->emplace(pair.first, pair.second);
        }

        try {
            const auto &sealOperationsPtr = m_userPtr->GetCustomSealOperations();
            const auto &cipherPtr = sealOperationsPtr->GetCipherFromBuffer(
                    GetUniqueStream(bookingResponse.total()));

            if (m_schemeType != seal::scheme_type::ckks) {
                const int noiseBudget = sealOperationsPtr->GetDecryptorPtr()->invariant_noise_budget(
                        *cipherPtr);

                if (noiseBudget == 0) {
                    throw std::runtime_error("Noise Budget cannot be zero");
                }
            }

            const auto &totalVar = sealOperationsPtr->Decrypt(*cipherPtr);
            NumVariantToAny(&totalVar, publicResponsePtr->mutable_total());
        } catch (const std::exception &e) {
            LOG4CPLUS_ERROR(*m_logger, "Error occurred during decryption.\nError Message: " + std::string(e.what()));
        }

        return publicResponsePtr;
    }

    std::unique_ptr<communication::sec::BookingRequest>
    ClientManager::MapJourneyToBookingRequest(const communication::Ride &ride, const bool isCKKS) const {

        auto requestPtr = GetUnique<communication::sec::BookingRequest>();
        const auto &customSealOperations = m_userPtr->GetCustomSealOperations();
        requestPtr->set_relinkeys(customSealOperations->GetRelinKeysBuffer());

        int bookingType = (GetRandomNumber<int>()) % (communication::BookingType_ARRAYSIZE - 1);
        requestPtr->set_bookingtype(static_cast<communication::BookingType>(bookingType));

        const auto &coefficientVariant = AnyToNumVariant(isCKKS, &ride.coefficient());
        const auto coefficientBuffer = customSealOperations->GetSymmetricEncryptedBuffer(coefficientVariant);
        requestPtr->set_coefficient(*coefficientBuffer);

        const auto &unitPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().unitprice());
        const auto unitPriceBuffer = customSealOperations->GetSymmetricEncryptedBuffer(unitPriceVariant);
        requestPtr->set_unitprice(*unitPriceBuffer);

        const auto &discountVariant = AnyToNumVariant(isCKKS, &ride.discount());
        if (GetAnyVariant<double>(&discountVariant) > 0) {
            const auto discountBuffer = customSealOperations->GetSymmetricEncryptedBuffer(discountVariant);
            requestPtr->set_discount(*discountBuffer);
        }

        const auto &seatPriceVariant = AnyToNumVariant(isCKKS, &ride.transporter().seatprice());
        if (GetAnyVariant<double>(&seatPriceVariant) > 0) {
            const auto seatPriceBuffer = customSealOperations->GetSymmetricEncryptedBuffer(seatPriceVariant);
            requestPtr->set_seatprice(*seatPriceBuffer);
        }

        return requestPtr;
    }

    void ClientManager::MapRideAndSeatNumberMap(google::protobuf::Map<std::string, int32_t> &targetMap,
                                                const google::protobuf::Map<std::string, int32_t> *sourceMapPtr) {
        for (const auto &pair: *sourceMapPtr) {
            targetMap.insert(pair);
        }
    }

} // yakbas