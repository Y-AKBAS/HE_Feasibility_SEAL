
#if (!DISABLE_TESTS)

#include "doctest/doctest.h"
#include "Utils.h"
#include "ClientManager.h"
#include <memory>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include "ApplicationConstants.h"
#include "Timer.h"
#include "SharedCommunication.pb.h"

namespace yakbas::pub::test {

    TEST_SUITE("Public Client Test Suite") {

        static void
        CheckJourneys(const std::vector<std::unique_ptr<communication::Journey>> &journeys, int numberOfJourneys);

        static std::uint64_t findTotal(communication::Journey &journey);

        const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

        TEST_CASE("Public Client Manager Initialization Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());
        }

        TEST_CASE("Client Manager Public Search Request Test") {

            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());
            Timer timer;
            const int numberOfJourneys = 500;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);
            CheckJourneys(*journeysVecPtr, numberOfJourneys);
            long long int passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "Public Search Request passed time in millis: " +
                           std::to_string(passedTimeInMillisWithStop));
        }

        TEST_CASE("Client Manager Public Booking Request Test") {

            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());
            Timer timer;
            const int numberOfJourneys = 20;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);

            for (int i = 0; i < numberOfJourneys; ++i) {

                const auto &journeyPtr = journeysVecPtr->at(i);
                std::uint64_t totalBeforeSent = findTotal(*journeyPtr);

                const auto bookingResponsePtr = clientManagerPtr->Book(*journeyPtr);

                CHECK(bookingResponsePtr->total() == totalBeforeSent);
            }
            long long int passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "Public Booking passed time in millis for " + std::to_string(numberOfJourneys) +
                           " journeys: " +
                           std::to_string(passedTimeInMillisWithStop));
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

        static std::uint64_t findTotal(communication::Journey &journey) {

            std::uint64_t total = 0;
            const auto &rides = journey.rides();
            CHECK(!rides.empty());

            for (const auto &ride: rides) {
                total += ride.coefficient() * ride.transporter().unitprice();
                total += ride.transporter().seatprice();
                total -= ride.discount();
            }

            return total;
        }
    }
}

#endif