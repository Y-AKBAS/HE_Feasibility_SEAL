
#if (!DISABLE_TESTS)

#include "Utils.h"
#include "MobilityProviderGenerator.h"
#include "CustomSealOperations.h"
#include "doctest/doctest.h"
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>


namespace yakbas::sec::test {

    TEST_SUITE("Secret Mobility Provider Test Suite") {

        TEST_CASE("Secret Mobility Provider Tests") {

            static const auto logger = util::GetUnique<log4cplus::Logger>(
                    log4cplus::Logger::getInstance("Test Logger"));
            static const auto sealOperationsPtr = util::GetUnique<CustomSealOperations>();

            SUBCASE("Secret Mobility Provider Generator Test") {
/*
                LOG4CPLUS_DEBUG(*logger, "Running Generator Test...");
                const std::string from = "Leipzig";
                const std::string to = "Halle";
                const int numberOfJourneys = 5;

                const auto requestPtr = util::GetUnique<communication::sec::SearchRequest>();
                requestPtr->set_from(from);
                requestPtr->set_to(to);
                requestPtr->set_numberofjourneys(numberOfJourneys);

                //const auto writer = util::GetUnique<grpc::ServerWriter<communication::sec::Journey>>();
                grpc::ServerWriter<communication::sec::Journey> writer =
                        new grpc::ServerWriterInterface<communication::sec::Journey>();
                auto responsePtr = util::GetUnique<communication::sec::SearchResponse>();
                auto status = MobilityProviderGenerator::GenerateJourneys(
                        requestPtr.get(),
                        responsePtr.get(),
                        *sealOperationsPtr->GetEncryptorPtr()
                );

                CHECK(status.ok());

                const google::protobuf::RepeatedPtrField<communication::sec::Journey> &repeatedPtrField
                        = responsePtr->journeys();

                CHECK(repeatedPtrField.size() == numberOfJourneys);

                std::for_each(repeatedPtrField.begin(),
                              repeatedPtrField.end(),
                              [&from, &to](const communication::sec::Journey &journey) {
                                  const auto &rides = journey.rides();

                                  std::for_each(rides.begin(),
                                                rides.end(),
                                                [&from, &to](const communication::sec::Ride &ride) {
                                                    CHECK(ride.from() == from);
                                                    CHECK(ride.to() == to);
                                                });
                              });*/
            }

        }
    }
}

#endif