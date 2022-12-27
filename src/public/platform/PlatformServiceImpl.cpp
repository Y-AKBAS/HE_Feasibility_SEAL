
#include <log4cplus/loggingmacros.h>
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
                const static bool isSet = [&bookingRequestPtr, &response]() -> bool {
                    response->set_invoicingclerktype(bookingRequestPtr->invoicingclerktype());
                    response->set_bookingtype(bookingRequestPtr->bookingtype());
                    return true;
                }();
                total += GetRequestTotalAndInsertSeat(*bookingRequestPtr, rideIdSeatNumberMap);
            }
        } while (isReadable);

        response->set_total(total);

        return grpc::Status::OK;
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


} // yakbas