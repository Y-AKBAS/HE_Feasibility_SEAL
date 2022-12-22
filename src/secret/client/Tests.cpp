
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

    TEST_SUITE("Secret Client Test Suite") {

        TEST_CASE("Secret Client Tests") {

            const static auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            SUBCASE("Client Manager Initialization Test") {
                const auto clientManagerPtr = std::make_unique<ClientManager>();
                CHECK(ClientManager::IsInitialized());
            }

            SUBCASE("Client Manager Search Request Test") {

                const auto clientManagerPtr = std::make_unique<ClientManager>();
                CHECK(ClientManager::IsInitialized());

                const int numberOfJourneys = 12;
                const auto journeysVecPtr = clientManagerPtr->DoSearchRequest("Leipzig", "Halle", numberOfJourneys);

                CHECK(!journeysVecPtr->empty());
                CHECK(journeysVecPtr->size() == numberOfJourneys);

                for (const auto &journeyPtr: *journeysVecPtr) {

                    const auto &rides = journeyPtr->rides();
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
    }
}

#endif