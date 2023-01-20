
#include <log4cplus/loggingmacros.h>
#include "MobilityProviderServiceImpl.h"
#include "MobilityProviderGenerator.h"
#include "Utils.h"

namespace yakbas::pub {
    using namespace yakbas::util;

    MobilityProviderServiceImpl::MobilityProviderServiceImpl(const std::string &&loggerInstance)
            : m_logger(std::make_unique<log4cplus::Logger>(
            log4cplus::Logger::getInstance(loggerInstance))) {}

    grpc::Status MobilityProviderServiceImpl::SearchForRides(grpc::ServerContext *context,
                                                             const communication::SearchRequest *request,
                                                             grpc::ServerWriter<communication::Journey> *writer) {

        LOG4CPLUS_DEBUG(*m_logger, "Public Mobility Provider Service impl SearchForRides invoked...");
        return MobilityProviderGenerator::GenerateJourneys(request, writer);
    }

    grpc::Status MobilityProviderServiceImpl::BookOnMobilityProvider(grpc::ServerContext *context,
                                                                     const communication::pub::BookingRequest *request,
                                                                     communication::BookingResponse *response) {

        auto rideIdSeatNumberMap = response->mutable_rideidseatnumbermap();
        response->set_invoicingclerktype(request->invoicingclerktype());
        response->set_bookingtype(request->bookingtype());
        std::uint64_t total = GetRequestTotalAndInsertSeat(*request, rideIdSeatNumberMap);
        auto anyPtr = response->mutable_total();
        NumToAny(total, anyPtr);

        return grpc::Status::OK;
    }

    std::uint64_t
    MobilityProviderServiceImpl::GetRequestTotalAndInsertSeat(const communication::pub::BookingRequest &request,
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

    grpc::Status MobilityProviderServiceImpl::ReportInvoicing(grpc::ServerContext *context,
                                                              const communication::InvoicingReport *request,
                                                              communication::InvoicingResponse *response) {

        // Normally the other information should be written to the db :)
        response->set_status(communication::StatusCode::SUCCESSFUL);
        return grpc::Status::OK;
    }

} // yakbas