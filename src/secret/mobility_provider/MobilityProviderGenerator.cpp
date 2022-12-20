
#include "MobilityProviderGenerator.h"
#include "Utils.h"
#include "SealOperations.h"
#include "Timer.h"

#include <google/protobuf/util/time_util.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    std::map<communication::TransporterType, communication::UnitPriceType>
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

    void MobilityProviderGenerator::GenerateRide(const communication::sec::SearchRequest *request,
                                                 const seal::Encryptor &encryptor,
                                                 communication::sec::Ride *ridePtr) {

        // optionals: seatPrice, discount

        const auto providerId = GetUUID();
        const auto randomNumber = GetRandomNumber();
        const auto transporterType = GetTransporterType(static_cast<int>(randomNumber));
        const bool isSeatPriceMeaningful = IsSeatPriceMeaningful(transporterType);
        std::unique_ptr<std::string> seatPriceBufferPtr{};

        //ciphers
        auto unitPriceBufferPtr = SealOperations::GetEncryptedBuffer(GetRandomNumber(), encryptor);
        const auto coefficientPtr = SealOperations::GetEncryptedBuffer(GetRandomNumber(), encryptor);
        //const auto discountRatePtr = SealOperations::GetEncryptedBuffer(GetRandomNumber(), encryptor);

        // set Timestamp
        const auto timestampPtr = ridePtr->mutable_starttime();
        timestampPtr->set_nanos(static_cast<int32_t>(Timer::GetCurrentTimeNanos()));

        // set Transporter
        const auto transporterPtr = ridePtr->mutable_transporter();
        transporterPtr->set_providerid(providerId);
        transporterPtr->set_unitprice(*unitPriceBufferPtr);
        transporterPtr->set_capacity(GetRandomNumber());
        transporterPtr->set_transportertype(transporterType);
        transporterPtr->set_unitpricetype(m_transporterUnitPriceType.find(transporterType)->second);

        // set seat price if it makes sense
        if (isSeatPriceMeaningful) {
            seatPriceBufferPtr = SealOperations::GetEncryptedBuffer(GetRandomNumber(), encryptor);
            //transporterPtr->set_seatprice(*seatPriceBufferPtr);
        }

        // set other infos
        ridePtr->set_rideid(GetUUID());
        ridePtr->set_providerid(providerId);
        ridePtr->set_from(request->from());
        ridePtr->set_to(request->to());
        ridePtr->set_coefficient(*coefficientPtr);
       // ridePtr->set_discountrate(*discountRatePtr);
    }

    communication::TransporterType MobilityProviderGenerator::GetTransporterType(int value) {
        return communication::TransporterType_IsValid(value) ?
               static_cast<communication::TransporterType>(value) :
               communication::TransporterType::AUTO;
    }

    bool MobilityProviderGenerator::IsSeatPriceMeaningful(const communication::TransporterType type) {
        return communication::TransporterType::REGIONAL_BAHN == type;
    }

    void MobilityProviderGenerator::GenerateRides(const communication::sec::SearchRequest *request,
                                                  const seal::Encryptor &encryptor,
                                                  communication::sec::Journey *journeyPtr,
                                                  const int numberOfRides) {

        for (int i = 0; i < numberOfRides; ++i) {
            communication::sec::Ride *ridesPtr = journeyPtr->add_rides();
            GenerateRide(request, encryptor, ridesPtr);
        }
    }

    grpc::Status MobilityProviderGenerator::GenerateJourneys(const communication::sec::SearchRequest *request,
                                                             communication::sec::SearchResponse *response,
                                                             const seal::Encryptor &encryptor,
                                                             int numberOfJourneys) {

        static const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

        try {
            for (int i = 0; i < numberOfJourneys; ++i) {
                communication::sec::Journey *journeyPtr = response->add_journeys();
                GenerateRides(request, encryptor, journeyPtr, (i % 2) + 1);
            }
        } catch (const std::exception &e) {
            const std::string message = "Setting Journeys has failed...";
            LOG4CPLUS_ERROR(*logger, message + "\n" + e.what());
            return {grpc::StatusCode::INTERNAL, message};
        }

        return {grpc::Status::OK};
    }

} // yakbas
