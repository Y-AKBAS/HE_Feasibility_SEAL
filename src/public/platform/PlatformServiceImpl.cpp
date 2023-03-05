
#include <log4cplus/loggingmacros.h>
#include <numeric>
#include "PlatformServiceImpl.h"
#include "ApplicationConstants.h"
#include "Utils.h"

namespace yakbas::pub {

    using namespace yakbas::util;

    PlatformServiceImpl::PlatformServiceImpl()
            : m_platformClientManager(std::make_unique<PlatformClientManager>()),
              m_logger(std::make_unique<log4cplus::Logger>(
                      log4cplus::Logger::getInstance("Secret Platform Service Impl"))) {}

    grpc::Status PlatformServiceImpl::CreateInvoice(grpc::ServerContext *context,
                                                    const communication::InvoicingRequest *request,
                                                    communication::InvoicingResponse *response) {

        // Implementation comes here
        return Service::CreateInvoice(context, request, response);
    }

    grpc::Status
    PlatformServiceImpl::SearchForRides(::grpc::ServerContext *context, const ::communication::SearchRequest *request,
                                        ::grpc::ServerWriter<::communication::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Public Platform Service impl SearchForRides invoked...");

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL_1);
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
            LOG4CPLUS_DEBUG(*m_logger, "Journeys sent successfully ... ");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during SearchForRides(). Error message: " + status.error_message());
        }

        return status;
    }

    grpc::Status PlatformServiceImpl::BookOnPlatform(grpc::ServerContext *context,
                                                     grpc::ServerReader<communication::pub::BookingRequest> *reader,
                                                     communication::BookingResponse *response) {

        response->set_journey_id(GetUUID());
        auto rideIdSeatNumberMap = response->mutable_rideidseatnumbermap();
        std::uint64_t total{};

        bool isReadable;
        do {
            const auto bookingRequestPtr = GetUnique<communication::pub::BookingRequest>();
            isReadable = reader->Read(bookingRequestPtr.get());
            if (isReadable) {
                response->set_invoicingclerktype(bookingRequestPtr->invoicingclerktype());
                response->set_bookingtype(bookingRequestPtr->bookingtype());
                total += GetRequestTotalAndInsertSeat(*bookingRequestPtr, rideIdSeatNumberMap);
            }
        } while (isReadable);

        auto anyPtr = response->mutable_total();
        NumToAny(total, anyPtr);

        return grpc::Status::OK;
    }

    grpc::Status PlatformServiceImpl::BookAsymmetricOnPlatform(grpc::ServerContext *context,
                                                               const communication::SearchRequest *request,
                                                               communication::BookingResponse *response) {

        response->set_journey_id(GetUUID());
        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL_1);
        grpc::ClientContext clientContext;
        const auto clientReaderPtr = stubPtr->SearchForRides(&clientContext, *request);
        std::vector<std::uint64_t> rideTotals{};

        bool isReadable;
        do {
            const auto journeyPtr = GetUnique<communication::Journey>();
            if ((isReadable = clientReaderPtr->Read(journeyPtr.get()))) {
                for (const auto &ride: journeyPtr->rides()) {
                    if (auto rideTotal = FindRideTotal(ride)) {
                        rideTotals.push_back(rideTotal);
                    } else {
                        return {grpc::StatusCode::INTERNAL,
                                "Public Platform BookAsymmetricOnPlatform computation failed..."};
                    }
                }
            }
        } while (isReadable);

        const grpc::Status &status = clientReaderPtr->Finish();

        if (status.ok()) {
            LOG4CPLUS_DEBUG(*m_logger, "BookAsymmetricOnPlatform invoked successfully ... ");
        } else {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error occurred during BookAsymmetricOnPlatform(). Error message: " +
                            status.error_message());
            throw std::runtime_error(status.error_message());
        }

        const auto total = std::accumulate(rideTotals.begin(), rideTotals.end(), 0ULL);
        NumToAny(total, response->mutable_total());

        return status;
    }

    grpc::Status PlatformServiceImpl::BookOnMobilityProviders(grpc::ServerContext *context,
                                                              grpc::ServerReader<communication::pub::BookingRequest> *reader,
                                                              communication::BookingResponse *response) {

        response->set_journey_id(GetUUID());
        auto rideIdSeatNumberMap = response->mutable_rideidseatnumbermap();
        std::uint64_t total{};

        const std::unique_ptr<publicService::Stub> &stub_1 = m_platformClientManager->GetStub(
                constants::MOBILITY_PROVIDER_CHANNEL_1);
        const std::unique_ptr<publicService::Stub> &stub_2 = m_platformClientManager->GetStub(
                constants::MOBILITY_PROVIDER_CHANNEL_2);

        bool isReadable;
        int count{};
        do {
            ++count;
            const auto bookingRequestPtr = GetUnique<communication::pub::BookingRequest>();
            isReadable = reader->Read(bookingRequestPtr.get());
            if (isReadable) {
                HandleIsReadable(rideIdSeatNumberMap, total, stub_1, stub_2, count, bookingRequestPtr);
            }
        } while (isReadable);

        NumToAny(total, response->mutable_total());

        return grpc::Status::OK;
    }

    grpc::Status PlatformServiceImpl::ReportInvoicing(grpc::ServerContext *context,
                                                      const communication::InvoicingReport *request,
                                                      communication::InvoicingResponse *response) {

        const auto stubPtr = m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL_1);
        grpc::ClientContext clientContext;
        const auto &status = stubPtr->ReportInvoicing(&clientContext, *request, response);

        if (!status.ok()) {
            throw std::runtime_error("Reporting public invoice failed in Secret Platform");
        }

        response->set_status(communication::StatusCode::SUCCESSFUL);
        return status;
    }

    void
    PlatformServiceImpl::HandleIsReadable(google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap,
                                          uint64_t &total,
                                          const std::unique_ptr<publicService::Stub> &stub_1,
                                          const std::unique_ptr<publicService::Stub> &stub_2, int &count,
                                          const std::unique_ptr<communication::pub::BookingRequest> &bookingRequestPtr) const {

        grpc::ClientContext clientContext{};
        communication::BookingResponse localResponse{};
        grpc::Status localStatus{};

        if (count % 2 == 0) {
            localStatus = stub_1->BookOnMobilityProvider(&clientContext, *bookingRequestPtr, &localResponse);
        } else {
            localStatus = stub_2->BookOnMobilityProvider(&clientContext, *bookingRequestPtr, &localResponse);
        }

        if (localStatus.ok()) {
            total += GetRequestTotalAndInsertSeat(*bookingRequestPtr, rideIdSeatNumberMap);
        } else {
            LOG4CPLUS_ERROR(*m_logger, "Handling HandleIsReadable failed. Reason: " + localStatus.error_message());
            throw std::runtime_error(localStatus.error_message());
        }
    }

    std::uint64_t PlatformServiceImpl::GetRequestTotalAndInsertSeat(const communication::pub::BookingRequest &request,
                                                                    google::protobuf::Map<std::string, int32_t> *rideIdSeatNumberMap) {

        std::uint64_t requestTotal = request.unitprice() * request.coefficient();

        const auto &discount = request.discount();
        if (discount > 0) {
            requestTotal -= discount;
        }

        const auto &seatPrice = request.seatprice();
        if (seatPrice > 0) {
            requestTotal += seatPrice;
            rideIdSeatNumberMap->emplace(request.ride_id(), request.seatnumber());
        }

        return requestTotal;
    }

    grpc::Status
    PlatformServiceImpl::StartUsing(grpc::ServerContext *context, const communication::StartUsingRequest *request,
                                    communication::StartUsingResponse *response) {
        grpc::ClientContext clientContext;
        return m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL_2)->StartUsing(&clientContext,
                                                                                                    *request, response);
    }

    grpc::Status
    PlatformServiceImpl::EndUsing(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                                  communication::EndUsingResponse *response) {
        grpc::ClientContext clientContext;
        return m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL_2)->EndUsing(&clientContext,
                                                                                                  *request, response);
    }

    grpc::Status PlatformServiceImpl::ReportUsageTotal(grpc::ServerContext *context,
                                                       const communication::UsageTotalReportRequest *request,
                                                       communication::UsageTotalReportResponse *response) {
        grpc::ClientContext clientContext;
        return m_platformClientManager->GetStub(constants::MOBILITY_PROVIDER_CHANNEL_2)->ReportUsageTotal(
                &clientContext, *request, response);
    }

    std::uint64_t PlatformServiceImpl::FindRideTotal(const communication::Ride &ride) {

        std::uint64_t requestTotal = AnyToNum<std::uint64_t>(&ride.transporter().unitprice()) *
                                     AnyToNum<std::uint64_t>(&ride.coefficient());

        const auto &discount = AnyToNum<std::uint64_t>(&ride.discount());
        if (discount > 0) {
            requestTotal -= discount;
        }

        const auto &seatPrice = AnyToNum<std::uint64_t>(&ride.transporter().seatprice());
        if (seatPrice > 0) {
            requestTotal += seatPrice;
        }

        return requestTotal;
    }

} // yakbas