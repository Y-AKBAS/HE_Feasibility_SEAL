
#pragma once

#include "SecretCommunication.grpc.pb.h"
#include "SharedCommunication.pb.h"

#include <vector>
#include <memory>
#include <log4cplus/logger.h>


namespace yakbas::pub {
    class MobilityProviderGenerator {
    public:

        static communication::TransporterType GetTransporterType(int value);

        static bool IsSeatPriceMeaningful(communication::TransporterType type);

        static void GenerateRide(const communication::SearchRequest *request,
                                 communication::Ride *ridePtr);

        static void GenerateRides(const communication::SearchRequest *request,
                                  communication::Journey *journeyPtr,
                                  int numberOfRides);

        [[nodiscard]] static grpc::Status GenerateJourneys(const communication::SearchRequest *request,
                                                           grpc::ServerWriter<communication::Journey> *writer);

    private:
        static const std::map<communication::TransporterType, communication::UnitPriceType> m_transporterUnitPriceType;
        static const std::unique_ptr<log4cplus::Logger> m_logger;
    };

} // yakbas
