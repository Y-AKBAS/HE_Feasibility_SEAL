
#ifndef DISABLE_TESTS
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "UserSealOperations.h"
#include "Utils.h"
#include <memory>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec::test {

    TEST_SUITE("Seal Util Test Suit") {

        TEST_CASE("Seal Util Tests") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            SUBCASE("GetOperationsTest") {
                const auto sealKeys_1 = util::GetUnique<SealKeys>(
                        seal::scheme_type::bfv, 16384, 1024
                );

                const auto userSealOperations =
                        util::GetUnique<UserSealOperations>(*sealKeys_1);
                const SealOperations &operations_1 = UserSealOperations::GetOperations(*sealKeys_1);
                const SealOperations &operations_1_1 = UserSealOperations::GetOperations(*sealKeys_1);
                CHECK(operations_1 == operations_1_1);
                CHECK(*operations_1.GetSealInfoPtr() == *operations_1_1.GetSealInfoPtr());
            }

            SUBCASE("Encryption Test") {
                const auto userSealOperations = util::GetUnique<UserSealOperations>();
                std::uint64_t num = util::GetRandomNumber();
                CHECK(num < 15);

                const auto cipherTextPtr = userSealOperations->Encrypt(num);
                CHECK(num == userSealOperations->Decrypt(*cipherTextPtr));
            }

            SUBCASE("Serialization Test") {
                const auto userSealOperations = util::GetUnique<UserSealOperations>();

                std::uint64_t num = util::GetRandomNumber();
                CHECK(num < 15);

                const auto cipherStringPtr = userSealOperations->GetEncryptedBuffer(num);
                const auto stream = util::GetUnique<std::stringstream>(*cipherStringPtr);

                const auto newCipher = userSealOperations->GetCipherFromBuffer(stream);
                CHECK(num == userSealOperations->Decrypt(*newCipher));
            }

            SUBCASE("Serialization Test 2") {
                const auto userSealOperations = util::GetUnique<UserSealOperations>();

                std::uint64_t num = util::GetRandomNumber();
                CHECK(num < 15);

                const auto cipherStringPtr = userSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                CHECK(num == userSealOperations->DecryptFromBuffer(stream));
            }

            ::log4cplus::deinitialize();
        }
    }

} // yakbas

#endif
#endif