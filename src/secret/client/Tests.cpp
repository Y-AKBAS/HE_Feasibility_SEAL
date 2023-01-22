
#if (!DISABLE_TESTS)

#include "doctest/doctest.h"
#include "CustomSealOperations.h"
#include "ClientManager.h"
#include <memory>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include "Timer.h"

namespace yakbas::sec::test {
    using namespace yakbas::util;

    TEST_SUITE("Secret Client Test Suite") {

        static void
        CheckJourneys(const std::vector<std::unique_ptr<communication::Journey>> &journeys, int numberOfJourneys,
                      bool isCKKS);

        static double findTotal(communication::Journey &journey, bool isCKKS);

        const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

        TEST_CASE("Client Manager Initialization Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            CHECK(ClientManager::IsInitialized());
        }

        TEST_CASE("Client Manager Search Request Test") {

            const auto clientManagerPtr = std::make_unique<ClientManager>();
            const bool isCKKS = clientManagerPtr->GetSchemeType() == seal::scheme_type::ckks;
            CHECK(ClientManager::IsInitialized());

            const int numberOfJourneys = 12;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);
            CheckJourneys(*journeysVecPtr, numberOfJourneys, isCKKS);
        }

        TEST_CASE("Client Manager Secret Search Request Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            const bool isCKKS = clientManagerPtr->GetSchemeType() == seal::scheme_type::ckks;
            CHECK(ClientManager::IsInitialized());

            const int numberOfJourneys = 5;
            const auto journeysVecPtr = clientManagerPtr->SearchSecretly("Leipzig", "Halle",
                                                                         numberOfJourneys);
            CheckJourneys(*journeysVecPtr, numberOfJourneys, isCKKS);
        }

        TEST_CASE("Client Manager Secret Booking Request Test") {

            const auto clientManagerPtr = std::make_unique<ClientManager>();
            const bool isCKKS = clientManagerPtr->GetSchemeType() == seal::scheme_type::ckks;
            CHECK(ClientManager::IsInitialized());
            Timer timer;
            const int numberOfJourneys = 8;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);

            for (int i = 0; i < numberOfJourneys; ++i) {

                const auto &journeyPtr = journeysVecPtr->at(i);
                double totalBeforeSent = findTotal(*journeyPtr, isCKKS);

                const auto bookingResponsePtr = clientManagerPtr->BookSecretlyAndDecrypt(*journeyPtr);

                const double total = AnyToNum(isCKKS, &bookingResponsePtr->total());
                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&total, &totalBeforeSent));
                } else {
                    CHECK(total == totalBeforeSent);
                }
            }

            long long int passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "Secret Booking passed time in millis: " + std::to_string(passedTimeInMillisWithStop));
        }

        TEST_CASE("Client Manager Symmetric Secret Booking Request Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            const bool isCKKS = clientManagerPtr->GetSchemeType() == seal::scheme_type::ckks;
            CHECK(ClientManager::IsInitialized());
            Timer timer;
            const int numberOfJourneys = 8;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);

            for (int i = 0; i < numberOfJourneys; ++i) {
                const auto &journeyPtr = journeysVecPtr->at(i);
                double totalBeforeSent = findTotal(*journeyPtr, isCKKS);
                const auto bookingResponsePtr = clientManagerPtr->BookSymmetricSecretlyAndDecrypt(*journeyPtr);

                const double total = AnyToNum(isCKKS, &bookingResponsePtr->total());
                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&total, &totalBeforeSent));
                } else {
                    CHECK(total == totalBeforeSent);
                }
            }
            long long int passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "Secret Symmetric Secret Booking passed time in millis for " +
                           std::to_string(numberOfJourneys) +
                           " journeys: " +
                           std::to_string(passedTimeInMillisWithStop));

            timer.start();
            for (int i = 0; i < numberOfJourneys; ++i) {
                const auto &journeyPtr = journeysVecPtr->at(i);
                double totalBeforeSent = findTotal(*journeyPtr, isCKKS);
                const auto bookingResponsePtr = clientManagerPtr->BookSecretlyAndDecrypt(*journeyPtr);

                const double total = AnyToNum(isCKKS, &bookingResponsePtr->total());
                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&total, &totalBeforeSent));
                } else {
                    CHECK(total == totalBeforeSent);
                }
            }
            passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "Secret Booking passed time in millis for " + std::to_string(numberOfJourneys) +
                           " journeys: " +
                           std::to_string(passedTimeInMillisWithStop));
        }

        TEST_CASE("Client Manager BookOnOthers Request Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();
            const bool isCKKS = clientManagerPtr->GetSchemeType() == seal::scheme_type::ckks;
            CHECK(ClientManager::IsInitialized());
            Timer timer;
            const int numberOfJourneys = 8;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);

            for (int i = 0; i < numberOfJourneys; ++i) {
                const auto &journeyPtr = journeysVecPtr->at(i);
                double totalBeforeSent = findTotal(*journeyPtr, isCKKS);
                const auto bookingResponsePtr = clientManagerPtr->BookSecretlyOnOthersAndDecrypt(*journeyPtr);

                const double total = AnyToNum(isCKKS, &bookingResponsePtr->total());
                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&total, &totalBeforeSent));
                } else {
                    CHECK(total == totalBeforeSent);
                }
            }
            long long int passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "BookSecretlyOnOthersAndDecrypt passed time in millis for " +
                           std::to_string(numberOfJourneys) +
                           " journeys: " +
                           std::to_string(passedTimeInMillisWithStop));

            timer.start();
            for (int i = 0; i < numberOfJourneys; ++i) {
                const auto &journeyPtr = journeysVecPtr->at(i);
                double totalBeforeSent = findTotal(*journeyPtr, isCKKS);
                const auto bookingResponsePtr = clientManagerPtr->BookSymmetricSecretlyOnOthersAndDecrypt(*journeyPtr);

                const double total = AnyToNum(isCKKS, &bookingResponsePtr->total());
                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&total, &totalBeforeSent));
                } else {
                    CHECK(total == totalBeforeSent);
                }
            }
            passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "BookSymmetricSecretlyOnOthersAndDecrypt passed time in millis for " +
                           std::to_string(numberOfJourneys) +
                           " journeys: " +
                           std::to_string(passedTimeInMillisWithStop));
        }

        TEST_CASE("Client Manager Send And Stop Using Request Test") {
            const auto clientManagerPtr = std::make_unique<ClientManager>();

            Timer timer{};
            for (int i = 0; i < 10; ++i) {
                auto startUsingResponsePtr = clientManagerPtr->SendStartUsingRequest(true);
                CHECK(startUsingResponsePtr->status() == communication::SUCCESSFUL);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                auto endUsingResponsePtr = clientManagerPtr->SendEndUsingRequestAndDecrypt(true);
                CHECK(endUsingResponsePtr->status() == communication::SUCCESSFUL);
            }
            auto passedTime = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger, "Passed time in start-end using symmetric encrytpted requests: " +
                                    std::to_string(passedTime));

            timer.start();
            for (int i = 0; i < 10; ++i) {
                auto startUsingResponsePtr = clientManagerPtr->SendStartUsingRequest(false);
                CHECK(startUsingResponsePtr->status() == communication::SUCCESSFUL);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                auto endUsingResponsePtr = clientManagerPtr->SendEndUsingRequestAndDecrypt(false);
                CHECK(endUsingResponsePtr->status() == communication::SUCCESSFUL);
            }
            passedTime = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger, "Passed time in start-end using requests: " + std::to_string(passedTime));
        }

        TEST_CASE("Client Manager Payment Request Test") {

            const auto clientManagerPtr = std::make_unique<ClientManager>();
            const bool isCKKS = clientManagerPtr->GetSchemeType() == seal::scheme_type::ckks;
            CHECK(ClientManager::IsInitialized());
            Timer timer;
            const int numberOfJourneys = 8;
            const auto journeysVecPtr = clientManagerPtr->Search("Leipzig", "Halle", numberOfJourneys);

            const auto index = GetRandomNumber<int>() % numberOfJourneys;
            const auto &journeyPtr = journeysVecPtr->at(index);

            double totalBeforeSent = findTotal(*journeyPtr, isCKKS);
            const auto bookingResponsePtr = clientManagerPtr->BookSecretlyAndDecrypt(*journeyPtr);

            const double total = AnyToNum(isCKKS, &bookingResponsePtr->total());
            if (isCKKS) {
                CHECK(CompareWithDecimalTolerance(&total, &totalBeforeSent, 3));
            } else {
                CHECK(total == totalBeforeSent);
            }

            const auto invoicingResponsePtr = clientManagerPtr->Pay(*bookingResponsePtr);
            CHECK(invoicingResponsePtr->status() == communication::StatusCode::SUCCESSFUL);

            const auto reportInvoicingCode = clientManagerPtr->ReportInvoicing(*invoicingResponsePtr,
                                                                               *bookingResponsePtr);
            CHECK(reportInvoicingCode == communication::StatusCode::SUCCESSFUL);
            long long int passedTimeInMillisWithStop = timer.PassedTimeInMillisWithStop();
            LOG4CPLUS_INFO(*logger,
                           "Payment Request passed time in millis: " + std::to_string(passedTimeInMillisWithStop));
        }

        TEST_CASE("FindTotal test") {
            communication::Journey journey;
            const auto rides = journey.mutable_rides();
            auto ridePtr = rides->Add();
            const auto num = GetRandomNumber<std::uint64_t>();
            NumToAny(num, ridePtr->mutable_discount());
            NumToAny(num, ridePtr->mutable_coefficient());
            auto transporterPtr = ridePtr->mutable_transporter();
            NumToAny(num, transporterPtr->mutable_seatprice());
            NumToAny(num, transporterPtr->mutable_unitprice());
            auto total = ((num * num) + num) - num;
            CHECK(total == findTotal(journey, false));
        }

        TEST_CASE("FindTotal With NumVariant std::uint64_t test") {
            communication::Journey journey;
            const auto rides = journey.mutable_rides();
            auto ridePtr = rides->Add();
            const auto variant = GetRandomNumberVariant<std::uint64_t>();
            NumVariantToAny(&variant, ridePtr->mutable_discount());
            NumVariantToAny(&variant, ridePtr->mutable_coefficient());
            auto transporterPtr = ridePtr->mutable_transporter();
            NumVariantToAny(&variant, transporterPtr->mutable_seatprice());
            NumVariantToAny(&variant, transporterPtr->mutable_unitprice());
            const auto num = GetAnyVariant<std::uint64_t>(&variant);
            auto total = ((num * num) + num) - num;
            CHECK(total == findTotal(journey, false));
        }

        TEST_CASE("FindTotal With NumVariant double test") {
            communication::Journey journey;
            const auto rides = journey.mutable_rides();
            auto ridePtr = rides->Add();
            const auto variant = GetRandomNumberVariant<std::uint64_t>();
            NumVariantToAny(&variant, ridePtr->mutable_discount());
            NumVariantToAny(&variant, ridePtr->mutable_coefficient());
            auto transporterPtr = ridePtr->mutable_transporter();
            NumVariantToAny(&variant, transporterPtr->mutable_seatprice());
            NumVariantToAny(&variant, transporterPtr->mutable_unitprice());
            const auto num = GetAnyVariant<double>(&variant);
            auto total = ((num * num) + num) - num;
            CHECK(total == findTotal(journey, false));
        }

        void CheckJourneys(const std::vector<std::unique_ptr<communication::Journey>> &journeys, int numberOfJourneys,
                           bool isCKKS) {

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
                    CHECK(AnyToNum(isCKKS, &ride.coefficient()) >= constants::APP_MIN_RANDOM_NUMBER);
                    CHECK(AnyToNum(isCKKS, &ride.coefficient()) <= constants::APP_MAX_RANDOM_NUMBER);
                    CHECK(!ride.transporter().providerid().empty());
                    CHECK(AnyToNum(isCKKS, &ride.transporter().unitprice()) >= constants::APP_MIN_RANDOM_NUMBER);
                    CHECK(AnyToNum(isCKKS, &ride.transporter().unitprice()) <= constants::APP_MAX_RANDOM_NUMBER);
                }
            }
        }

        static double findTotal(communication::Journey &journey, bool isCKKS) {

            double total = 0.0;
            const auto &rides = journey.rides();
            CHECK(!rides.empty());

            for (const auto &ride: rides) {
                const double findTotalCoefficient = AnyToNum(isCKKS, &ride.coefficient());
                const double findTotalUnitPrice = AnyToNum(isCKKS, &ride.transporter().unitprice());
                const double findTotalSeatPrice = AnyToNum(isCKKS, &ride.transporter().seatprice());
                const double findTotalDiscount = AnyToNum(isCKKS, &ride.discount());

                total += findTotalCoefficient * findTotalUnitPrice;
                total += findTotalSeatPrice;
                total -= findTotalDiscount;
            }

            return total;
        }
    }
}

#endif