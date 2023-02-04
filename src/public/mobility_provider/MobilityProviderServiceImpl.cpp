
#include <log4cplus/loggingmacros.h>
#include "MobilityProviderServiceImpl.h"
#include "MobilityProviderGenerator.h"
#include "Utils.h"
#include "Timer.h"

namespace yakbas::pub {
    using namespace yakbas::util;

    std::string FindLoggerName(const std::string &loggerInstance) {
        const char &i = loggerInstance.at(loggerInstance.size() - 1);
        return "Public Mobility Provider Client Manager " + std::to_string(i);
    }

    MobilityProviderServiceImpl::MobilityProviderServiceImpl(const std::string &&loggerInstance)
            : m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance(loggerInstance))),
              m_clientManager(std::make_unique<MobilityProviderClientManager>(FindLoggerName(loggerInstance))) {}

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

    grpc::Status MobilityProviderServiceImpl::StartUsing(grpc::ServerContext *context,
                                                         const communication::StartUsingRequest *request,
                                                         communication::StartUsingResponse *response) {
        grpc::ClientContext clientContext;
        const auto &status = m_clientManager->GetStub(constants::TRANSPORT_CHANNEL)->StartUsing(&clientContext,
                                                                                                *request, response);
        if (!status.ok()) {
            response->set_status(communication::FAILED);
            throw std::runtime_error("Start Using Request failed in " + m_logger->getName());
        }

        response->set_status(communication::SUCCESSFUL);
        response->set_start_time_in_minutes(Timer::GetCurrentTimeMinutes());
        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::EndUsing(grpc::ServerContext *context,
                                                       const communication::EndUsingRequest *request,
                                                       communication::EndUsingResponse *response) {
        grpc::ClientContext clientContext;
        const auto status = m_clientManager->GetStub(constants::TRANSPORT_CHANNEL)->EndUsing(&clientContext, *request,
                                                                                             response);
        if (!status.ok()) {
            throw std::runtime_error("End Using Request failed in " + m_logger->getName());
        }

        response->set_end_time_in_minutes(Timer::GetCurrentTimeMinutes() + util::GetRandomNumber<int>());
        NumToAny<std::uint64_t>(response->mutable_unit_price());

        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::ReportUsageTotal(grpc::ServerContext *context,
                                                               const communication::UsageTotalReportRequest *request,
                                                               communication::UsageTotalReportResponse *response) {
        try {
            LOG4CPLUS_INFO(*m_logger, std::string("Report usage total: ") +
                                      std::to_string(AnyToNum<std::uint64_t>(&request->total())));
        } catch (std::exception &e) {
            LOG4CPLUS_ERROR(*m_logger, std::string("Exception during decryption. Message: ") + e.what());
            throw std::runtime_error(e.what());
        }

        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderServiceImpl::ReportInvoicing(grpc::ServerContext *context,
                                                              const communication::InvoicingReport *request,
                                                              communication::InvoicingResponse *response) {

        // Normally the other information should be written to the db :)
        response->set_status(communication::StatusCode::SUCCESSFUL);
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

} // yakbas