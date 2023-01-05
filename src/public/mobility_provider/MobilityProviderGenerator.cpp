
#include "MobilityProviderGenerator.h"
#include "Utils.h"
#include "Timer.h"

#include <google/protobuf/util/time_util.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::pub {
    using namespace yakbas::util;

    const std::map<communication::TransporterType, communication::UnitPriceType>
            MobilityProviderGenerator::m_transporterUnitPriceType{
            {communication::TransporterType::AUTO,          communication::UnitPriceType::KM},
            {communication::TransporterType::SHARED_AUTO,   communication::UnitPriceType::KM},
            {communication::TransporterType::TAXI,          communication::UnitPriceType::KM},
            {communication::TransporterType::BICYCLE,       communication::UnitPriceType::MIN},
            {communication::TransporterType::E_ROLLER,      communication::UnitPriceType::MIN},
            {communication::TransporterType::SCOOTER,       communication::UnitPriceType::MIN},
            {communication::TransporterType::BUS,           communication::UnitPriceType::FARE_ZONE},
            {communication::TransporterType::REGIONAL_BAHN, communication::UnitPriceType::FARE_ZONE},
            {communication::TransporterType::S_BAHN,        communication::UnitPriceType::FARE_ZONE},
            {communication::TransporterType::TRAM,          communication::UnitPriceType::FARE_ZONE}
    };

    const std::unique_ptr<log4cplus::Logger> MobilityProviderGenerator::m_logger =
            util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("Secret Mobility Provider Generator"));

    grpc::Status MobilityProviderGenerator::GenerateJourneys(const communication::SearchRequest *request,
                                                             grpc::ServerWriter<communication::Journey> *writer) {
        try {
            for (int i = 0; i < request->numberofjourneys(); ++i) {
                const auto journeyPtr = GetUnique<communication::Journey>();
                GenerateRides(request, journeyPtr.get(), (i % 2) + 1);
                writer->Write(*journeyPtr);
            }
        } catch (const std::exception &e) {
            const std::string message = "Setting Journeys has failed...";
            LOG4CPLUS_ERROR(*m_logger, message + "\n" + e.what());
            return {grpc::StatusCode::INTERNAL, message};
        }

        return grpc::Status::OK;
    }

    void MobilityProviderGenerator::GenerateRides(const communication::SearchRequest *request,
                                                  communication::Journey *journeyPtr,
                                                  const int numberOfRides) {

        for (int i = 0; i < numberOfRides; ++i) {
            communication::Ride *ridesPtr = journeyPtr->add_rides();
            GenerateRide(request, ridesPtr);
        }
    }

    void MobilityProviderGenerator::GenerateRide(const communication::SearchRequest *request,
                                                 communication::Ride *ridePtr) {

        // optionals: seatPrice, discount
        const auto providerId = GetUUID();
        const int randomInt = GetRandomNumber<int>();
        const auto transporterType = GetTransporterType(randomInt);
        const bool isSeatPriceMeaningful = IsSeatPriceMeaningful(transporterType);

        // set Timestamp
        const auto timestampPtr = ridePtr->mutable_starttime();
        timestampPtr->set_nanos(static_cast<int32_t>(Timer::GetCurrentTimeNanos()));

        google::protobuf::Any any{};
        NumToAny<std::uint64_t>(&any);

        // set Transporter
        const auto transporterPtr = ridePtr->mutable_transporter();
        transporterPtr->set_providerid(providerId);
        transporterPtr->mutable_unitprice()->CopyFrom(any);
        transporterPtr->set_capacity(GetRandomNumber<std::uint64_t>());
        transporterPtr->set_transportertype(transporterType);
        transporterPtr->set_unitpricetype(m_transporterUnitPriceType.find(transporterType)->second);

        // set seat price if it makes sense
        if (isSeatPriceMeaningful) {
            transporterPtr->mutable_seatprice()->CopyFrom(any);
        }

        // set other infos
        ridePtr->set_rideid(GetUUID());
        ridePtr->set_providerid(providerId);
        ridePtr->set_from(request->from());
        ridePtr->set_to(request->to());
        ridePtr->mutable_coefficient()->CopyFrom(any);

        if ((randomInt % 2) == 1) {
            ridePtr->mutable_discount()->CopyFrom(any);
        }
    }

    communication::TransporterType MobilityProviderGenerator::GetTransporterType(int value) {
        return communication::TransporterType_IsValid(value) ?
               static_cast<communication::TransporterType>(value) :
               communication::TransporterType::AUTO;
    }

    bool MobilityProviderGenerator::IsSeatPriceMeaningful(const communication::TransporterType type) {
        return communication::TransporterType::REGIONAL_BAHN == type;
    }

} // yakbas
