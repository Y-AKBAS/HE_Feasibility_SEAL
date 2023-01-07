
#pragma once

#include "SecretCommunication.grpc.pb.h"
#include "SharedCommunication.pb.h"
#include "Utils.h"
#include "SealOperations.h"

#include <vector>
#include <memory>
#include <seal/encryptor.h>
#include <log4cplus/logger.h>


namespace yakbas::sec {
    class MobilityProviderGenerator {
    public:

        static communication::TransporterType GetTransporterType(int value);

        static bool IsSeatPriceMeaningful(communication::TransporterType type);

        static void GenerateRide(const communication::SearchRequest *request,
                                 const SealOperations &operations,
                                 communication::Ride *ridePtr);

        static void GenerateRides(const communication::SearchRequest *request,
                                  communication::Journey *journeyPtr,
                                  const SealOperations &operations,
                                  int numberOfRides);

        [[nodiscard]] static grpc::Status GenerateJourneys(const communication::SearchRequest *request,
                                                           grpc::ServerWriter<communication::Journey> *writer,
                                                           const SealOperations &operations);

        static void GenerateSecretRide(const communication::sec::SearchRequest *request,
                                       const SealOperations &operations,
                                       const seal::Encryptor &encryptor,
                                       communication::sec::Ride *ridePtr);

        static void GenerateSecretRides(const communication::sec::SearchRequest *request,
                                        const SealOperations &operations,
                                        const seal::Encryptor &encryptor,
                                        communication::sec::Journey *journeyPtr,
                                        int numberOfRides);

        [[nodiscard]] static grpc::Status GenerateSecretJourneys(const communication::sec::SearchRequest *request,
                                                                 grpc::ServerWriter<communication::sec::Journey> *writer,
                                                                 const SealOperations &operations,
                                                                 const seal::Encryptor &encryptor);

    private:
        static const std::map<communication::TransporterType, communication::UnitPriceType> m_transporterUnitPriceType;
        static const std::unique_ptr<log4cplus::Logger> m_logger;
    };

} // yakbas
