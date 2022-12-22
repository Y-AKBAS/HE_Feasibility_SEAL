
#if (!DISABLE_TESTS)

#include "doctest/doctest.h"
#include "CustomSealOperations.h"
#include "Utils.h"
#include "ClientManager.h"
#include <memory>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include "ApplicationConstants.h"

namespace yakbas::sec::test {

    void CheckJourneys(const std::vector<std::unique_ptr<communication::Journey>> &journeys, int numberOfJourneys);

    TEST_SUITE("Secret Client Test Suite") {

        const static auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

        TEST_CASE("Client Manager Initialization Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());
        }

        TEST_CASE("Client Manager Search Request Test") {

            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());

            const int numberOfJourneys = 12;
            const auto journeysVecPtr = clientManagerPtr->DoSearchRequest("Leipzig", "Halle", numberOfJourneys);
            CheckJourneys(*journeysVecPtr, numberOfJourneys);
        }

        TEST_CASE("Client Manager Secret Search Request Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());

            const int numberOfJourneys = 12;
            const auto journeysVecPtr = clientManagerPtr->DoSecretSearchRequest("Leipzig", "Halle",
                                                                                numberOfJourneys);
            CheckJourneys(*journeysVecPtr, numberOfJourneys);
        }
    }


    void CheckJourneys(const std::vector<std::unique_ptr<communication::Journey>> &journeys, int numberOfJourneys) {

        CHECK(!journeys.empty());
        CHECK(journeys.size() == numberOfJourneys);

        for (const auto &journey: journeys) {

            const auto &rides = journey->rides();
            CHECK(!rides.empty());

            for (const auto &ride: rides) {
                CHECK(ride.has_starttime());
                CHECK(!ride.providerid().empty());
                CHECK(!ride.from().empty());
                CHECK(!ride.to().empty());
                CHECK(ride.coefficient() >= constants::APP_MIN_RANDOM_NUMBER);
                CHECK(ride.coefficient() <= constants::APP_MAX_RANDOM_NUMBER);
                CHECK(!ride.transporter().providerid().empty());
                CHECK(ride.transporter().unitprice() >= constants::APP_MIN_RANDOM_NUMBER);
                CHECK(ride.transporter().unitprice() <= constants::APP_MAX_RANDOM_NUMBER);
            }
        }
    }

}

#endif