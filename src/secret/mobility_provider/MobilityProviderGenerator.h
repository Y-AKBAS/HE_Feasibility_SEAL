
#pragma once

#include "SecretCommunication.grpc.pb.h"
#include "SharedCommunication.pb.h"

#include <vector>
#include <memory>
#include <seal/encryptor.h>


namespace yakbas::sec {
    class MobilityProviderGenerator {
    public:

        static communication::TransporterType GetTransporterType(int value);

        static bool IsSeatPriceMeaningful(communication::TransporterType type);

        static void GenerateRide(const communication::sec::SearchRequest *request,
                                 const seal::Encryptor &encryptor,
                                 communication::sec::Ride *ridePtr);

        static void GenerateRides(const communication::sec::SearchRequest *request,
                                  const seal::Encryptor &encryptor,
                                  communication::sec::Journey *journeyPtr,
                                  int numberOfRides);

        static grpc::Status GenerateJourneys(const communication::sec::SearchRequest *request,
                                             communication::sec::SearchResponse *response,
                                             const seal::Encryptor &encryptor,
                                             int numberOfJourneys);

    private:

        static std::map<communication::TransporterType, communication::UnitPriceType> m_transporterUnitPriceType;

    };

} // yakbas
