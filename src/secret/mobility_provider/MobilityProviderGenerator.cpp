
#include "MobilityProviderGenerator.h"
#include "Timer.h"

#include <google/protobuf/wrappers.pb.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
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

    grpc::Status MobilityProviderGenerator::GenerateSecretJourneys(const communication::sec::SearchRequest *request,
                                                                   grpc::ServerWriter<communication::sec::Journey> *writer,
                                                                   const SealOperations &operations,
                                                                   const seal::Encryptor &encryptor) {
        try {
            for (int i = 0; i < request->numberofjourneys(); ++i) {
                const auto journeyPtr = GetUnique<communication::sec::Journey>();
                GenerateSecretRides(request, operations, encryptor, journeyPtr.get(), (i % 2) + 1);
                writer->Write(*journeyPtr);
            }
        } catch (const std::exception &e) {
            const std::string message = "Journey generation has failed...";
            LOG4CPLUS_ERROR(*m_logger, message + "\n" + e.what());
            return {grpc::StatusCode::INTERNAL, message};
        }

        return grpc::Status::OK;
    }

    grpc::Status MobilityProviderGenerator::GenerateJourneys(const communication::SearchRequest *request,
                                                             grpc::ServerWriter<communication::Journey> *writer,
                                                             const SealOperations &operations) {
        try {
            for (int i = 0; i < request->numberofjourneys(); ++i) {
                const auto journeyPtr = GetUnique<communication::Journey>();
                GenerateRides(request, journeyPtr.get(), operations, (i % 2) + 1);
                writer->Write(*journeyPtr);
            }
        } catch (const std::exception &e) {
            const std::string message = "Journey generation has failed...";
            LOG4CPLUS_ERROR(*m_logger, message + "\n" + e.what());
            return {grpc::StatusCode::INTERNAL, message};
        }

        return grpc::Status::OK;
    }

    void MobilityProviderGenerator::GenerateSecretRides(const communication::sec::SearchRequest *request,
                                                        const SealOperations &operations,
                                                        const seal::Encryptor &encryptor,
                                                        communication::sec::Journey *journeyPtr, int numberOfRides) {

        for (int i = 0; i < numberOfRides; ++i) {
            communication::sec::Ride *ridesPtr = journeyPtr->add_rides();
            GenerateSecretRide(request, operations, encryptor, ridesPtr);
        }
    }

    void MobilityProviderGenerator::GenerateRides(const communication::SearchRequest *request,
                                                  communication::Journey *journeyPtr,
                                                  const SealOperations &operations,
                                                  const int numberOfRides) {

        for (int i = 0; i < numberOfRides; ++i) {
            communication::Ride *ridesPtr = journeyPtr->add_rides();
            GenerateRide(request, operations, ridesPtr);
        }
    }

    void MobilityProviderGenerator::GenerateSecretRide(const communication::sec::SearchRequest *request,
                                                       const SealOperations &operations,
                                                       const seal::Encryptor &encryptor,
                                                       communication::sec::Ride *ridePtr) {
        // optionals: seatPrice, discount
        const auto providerId = GetUUID();
        const bool isCkks = operations.GetSealInfoPtr()->m_sealKeys.m_schemeType == seal::scheme_type::ckks;
        const auto randomNumber = isCkks ? GetRandomNumberVariant<double>() : GetRandomNumberVariant<std::uint64_t>();
        const int randomInt = GetRandomNumber<int>();
        const auto transporterType = GetTransporterType(randomInt);
        const bool isSeatPriceMeaningful = IsSeatPriceMeaningful(transporterType);

        //ciphers
        auto unitPricePtr = operations.GetEncryptedBuffer(randomNumber, encryptor);
        const auto coefficientPtr = operations.GetEncryptedBuffer(randomNumber, encryptor);

        // set Timestamp
        const auto timestampPtr = ridePtr->mutable_starttime();
        timestampPtr->set_nanos(static_cast<int32_t>(Timer::GetCurrentTimeNanos()));

        // set Transporter
        const auto transporterPtr = ridePtr->mutable_transporter();
        transporterPtr->set_providerid(providerId);
        transporterPtr->set_unitprice(*unitPricePtr);
        transporterPtr->set_capacity(GetRandomNumber<std::uint64_t>());
        transporterPtr->set_transportertype(transporterType);
        transporterPtr->set_unitpricetype(m_transporterUnitPriceType.find(transporterType)->second);

        // set seat price if it makes sense
        if (isSeatPriceMeaningful) {
            const auto seatPricePtr = operations.GetEncryptedBuffer(randomNumber, encryptor);
            transporterPtr->set_seatprice(*seatPricePtr);
        }

        // set other infos
        ridePtr->set_rideid(GetUUID());
        ridePtr->set_providerid(providerId);
        ridePtr->set_from(request->from());
        ridePtr->set_to(request->to());
        ridePtr->set_coefficient(*coefficientPtr);

        if ((randomInt % 2) == 1) {
            const auto discountPtr = operations.GetEncryptedBuffer(randomNumber, encryptor);
            ridePtr->set_discount(*discountPtr);
        }
    }

    void MobilityProviderGenerator::GenerateRide(const communication::SearchRequest *request,
                                                 const SealOperations &operations,
                                                 communication::Ride *ridePtr) {
        // optionals: seatPrice, discount
        const auto providerId = GetUUID();
        const bool isCkks = operations.GetSealInfoPtr()->m_sealKeys.m_schemeType == seal::scheme_type::ckks;
        const auto randomNumber = isCkks ? GetRandomNumberVariant<double>() : GetRandomNumberVariant<std::uint64_t>();
        const int randomInt = GetRandomNumber<int>();
        const auto transporterType = GetTransporterType(randomInt);
        const bool isSeatPriceMeaningful = IsSeatPriceMeaningful(transporterType);

        // set Timestamp
        const auto timestampPtr = ridePtr->mutable_starttime();
        timestampPtr->set_nanos(static_cast<int32_t>(Timer::GetCurrentTimeNanos()));

        // set Transporter
        const auto transporterPtr = ridePtr->mutable_transporter();
        transporterPtr->set_providerid(providerId);
        NumVariantToAny(&randomNumber, transporterPtr->mutable_unitprice());
        transporterPtr->set_capacity(GetRandomNumber<std::uint64_t>());
        transporterPtr->set_transportertype(transporterType);
        transporterPtr->set_unitpricetype(m_transporterUnitPriceType.find(transporterType)->second);

        // set seat price if it makes sense
        if (isSeatPriceMeaningful) {
            NumVariantToAny(&randomNumber, transporterPtr->mutable_seatprice());
        }

        // set other infos
        ridePtr->set_rideid(GetUUID());
        ridePtr->set_providerid(providerId);
        ridePtr->set_from(request->from());
        ridePtr->set_to(request->to());
        NumVariantToAny(&randomNumber, ridePtr->mutable_coefficient());

        if ((randomInt % 2) == 1) {
            NumVariantToAny(&randomNumber, ridePtr->mutable_discount());
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
