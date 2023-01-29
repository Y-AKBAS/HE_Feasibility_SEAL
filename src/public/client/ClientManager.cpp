
#include <log4cplus/loggingmacros.h>
#include "ClientManager.h"
#include "ApplicationConstants.h"
#include "ClientGenerator.h"
#include "Utils.h"

namespace yakbas::pub {
    using namespace yakbas::util;

    using publicService = communication::pub::PublicCommunicationService;

    ClientManager::ClientManager()
            : m_userPtr(ClientGenerator::GeneratePublicUser()),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("PublicClientManager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Public Client Manager is being initialized...");
            ClientManager::CreateChannels(
                    {{constants::INVOICE_CLERK_CHANNEL, PUBLIC_INVOICE_CLERK_SERVER_PORT},
                     {constants::PLATFORM_CHANNEL,      PUBLIC_PLATFORM_SERVER_PORT}}
            );
        });
    }

    std::once_flag ClientManager::m_isInitialized{};

    bool ClientManager::IsInitialized() {
        return !m_channelMap.empty();
    }

    std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
    ClientManager::Search(const std::string &from, const std::string &to, int numberOfJourneys) {

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
            LOG4CPLUS_INFO(*m_logger, "Fetched Journeys successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Search(). Error message: " + status.error_message());
        }

        return journeyVecPtr;
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookOnPlatform(const communication::Journey &journey) {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::BookingResponse>();
        const auto clientWriterPtr = stubPtr->BookOnPlatform(clientContextPtr.get(), responsePtr.get());

        const auto &rides = journey.rides();

        for (const auto &ride: rides) {

            const auto requestPtr = GetUnique<communication::pub::BookingRequest>();

            int bookingType = GetRandomNumber<int>() % (communication::BookingType_ARRAYSIZE - 1);
            requestPtr->set_bookingtype(static_cast<communication::BookingType>(bookingType));

            requestPtr->set_coefficient(AnyToNum<std::uint64_t>(&ride.coefficient()));
            requestPtr->set_unitprice(AnyToNum<std::uint64_t>(&ride.transporter().unitprice()));

            const auto seatPrice = AnyToNum<std::uint64_t>(&ride.transporter().seatprice());
            if (seatPrice > 0) {
                requestPtr->set_seatprice(seatPrice);
            }

            const auto discount = AnyToNum<std::uint64_t>(&ride.discount());
            if (discount > 0) {
                requestPtr->set_discount(discount);
            }

            if (!clientWriterPtr->Write(*requestPtr)) {
                throw std::bad_function_call();
            }
        }

        clientWriterPtr->WritesDone();
        const auto &status = clientWriterPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Sent Public BookingRequests successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Sending Public BookingRequests. Error message: " +
                            status.error_message());
        }

        return responsePtr;
    }

    std::unique_ptr<communication::BookingResponse>
    ClientManager::BookOnMobilityProviders(const communication::Journey &journey) {

        const auto stubPtr = this->GetStub(constants::PLATFORM_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto responsePtr = GetUnique<communication::BookingResponse>();
        const auto clientWriterPtr = stubPtr->BookOnMobilityProviders(clientContextPtr.get(), responsePtr.get());

        const auto &rides = journey.rides();

        for (const auto &ride: rides) {

            const auto requestPtr = GetUnique<communication::pub::BookingRequest>();

            int bookingType = GetRandomNumber<int>() % (communication::BookingType_ARRAYSIZE - 1);
            requestPtr->set_bookingtype(static_cast<communication::BookingType>(bookingType));

            requestPtr->set_coefficient(AnyToNum<std::uint64_t>(&ride.coefficient()));
            requestPtr->set_unitprice(AnyToNum<std::uint64_t>(&ride.transporter().unitprice()));

            const auto seatPrice = AnyToNum<std::uint64_t>(&ride.transporter().seatprice());
            if (seatPrice > 0) {
                requestPtr->set_seatprice(seatPrice);
            }

            const auto discount = AnyToNum<std::uint64_t>(&ride.discount());
            if (discount > 0) {
                requestPtr->set_discount(discount);
            }

            if (!clientWriterPtr->Write(*requestPtr)) {
                throw std::bad_function_call();
            }
        }

        clientWriterPtr->WritesDone();
        const auto &status = clientWriterPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Sent Public BookingRequestsOnOthers successfully...");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during Sending Public BookingRequestsOnOthers. Error message: " +
                            status.error_message());
        }

        return responsePtr;
    }

    std::unique_ptr<communication::InvoicingResponse>
    ClientManager::Pay(const communication::BookingResponse &bookingResponse) {

        const auto stubPtr = this->GetStub(constants::INVOICE_CLERK_CHANNEL);
        const auto clientContextPtr = GetUnique<grpc::ClientContext>();
        auto invoicingResponsePtr = GetUnique<communication::InvoicingResponse>();

        const auto invoicingRequestPtr = GetUnique<communication::InvoicingRequest>();
        const auto total = AnyToNum<std::uint64_t>(&bookingResponse.total());
        invoicingRequestPtr->set_price(static_cast<double>(total));
        const auto protoUserPtr = invoicingRequestPtr->mutable_user();
        m_userPtr->ToProto(protoUserPtr);

        const grpc::Status &status = stubPtr->CreateInvoice(clientContextPtr.get(), *invoicingRequestPtr,
                                                            invoicingResponsePtr.get());
        if (status.ok()) {
            LOG4CPLUS_INFO(*m_logger, "Sent InvoicingRequest successfully...");
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

    void ClientManager::MapRideAndSeatNumberMap(google::protobuf::Map<std::string, int32_t> &targetMap,
                                                const google::protobuf::Map<std::string, int32_t> *sourceMapPtr) {
        for (const auto &pair: *sourceMapPtr) {
            targetMap.insert(pair);
        }
    }

} // yakbas