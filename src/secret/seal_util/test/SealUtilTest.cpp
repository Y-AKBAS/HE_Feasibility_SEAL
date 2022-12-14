
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif

#include "doctest/doctest.h"

#if (!DISABLE_TESTS)

#include "CustomSealOperations.h"
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

                const auto customSealOperations =
                        util::GetUnique<CustomSealOperations>(*sealKeys_1);
                const SealOperations &operations_1 = CustomSealOperations::GetOperations(*sealKeys_1);
                const SealOperations &operations_1_1 = CustomSealOperations::GetOperations(*sealKeys_1);
                CHECK(operations_1 == operations_1_1);
                CHECK(*operations_1.GetSealInfoPtr() == *operations_1_1.GetSealInfoPtr());
            }

            SUBCASE("Encryption Test") {
                const auto customSealOperations = util::GetUnique<CustomSealOperations>();
                std::uint64_t num = util::GetRandomNumber();
                CHECK(num < 15);

                const auto cipherTextPtr = customSealOperations->Encrypt(num);
                CHECK(num == customSealOperations->Decrypt(*cipherTextPtr));
            }

            SUBCASE("Serialization Test") {
                const auto customSealOperations = util::GetUnique<CustomSealOperations>();

                std::uint64_t num = util::GetRandomNumber();
                CHECK(num < 15);

                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = util::GetUnique<std::stringstream>(*cipherStringPtr);

                const auto newCipher = customSealOperations->GetCipherFromBuffer(stream);
                CHECK(num == customSealOperations->Decrypt(*newCipher));
            }

            SUBCASE("Serialization Test 2") {
                const auto customSealOperations = util::GetUnique<CustomSealOperations>();

                std::uint64_t num = util::GetRandomNumber();
                CHECK(num < 15);

                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                CHECK(num == customSealOperations->DecryptFromBuffer(stream));
            }

            SUBCASE("SealKeys Initialization test") {
                SealKeys keys = {};
                CHECK(keys.m_schemeType == static_cast<const seal::scheme_type>(SEAL_SCHEME_TYPE));
                CHECK(keys.m_polyModulusDegree == SEAL_POLY_MODULUS_DEGREE);
                CHECK(keys.m_plainModulus == SEAL_PLAIN_MODULUS_DEGREE);
            }

            ::log4cplus::deinitialize();
        }
    }

} // yakbas

#endif

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#undef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
