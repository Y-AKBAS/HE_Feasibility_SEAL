
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif

#include "doctest/doctest.h"

#if (!DISABLE_TESTS)

#include "CustomSealOperations.h"
#include <memory>
#include <log4cplus/configurator.h>


namespace yakbas::sec::test {

    template<typename T = num_variant>
    void EncryptionTest(const CustomSealOperations &customSealOperations, const bool isCKKS) {
        const auto num = GetRandomNumber<T>();
        CHECK(num <= constants::APP_MAX_RANDOM_NUMBER);
        const auto cipherTextPtr = customSealOperations.Encrypt(num);
        const auto &variant = customSealOperations.Decrypt(*cipherTextPtr);
        const auto decNum = std::get<T>(variant);
        if (isCKKS) {
            CHECK(CompareWithDecimalTolerance<T>(&num, &decNum));
        } else {
            CHECK(num == decNum);
        }
    }

    TEST_SUITE("Seal Util Test Suit") {

        TEST_CASE("Seal Util Tests") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            SUBCASE("GetOperationsTest") {
                const auto sealKeys_1 = util::GetUnique<SealKeys>();

                const auto customSealOperations =
                        util::GetUnique<CustomSealOperations>(*sealKeys_1);
                const SealOperations &operations_1 = CustomSealOperations::GetOperations(*sealKeys_1);
                const SealOperations &operations_1_1 = CustomSealOperations::GetOperations(*sealKeys_1);
                CHECK(operations_1 == operations_1_1);
                CHECK(*operations_1.GetSealInfoPtr() == *operations_1_1.GetSealInfoPtr());
            }

            SUBCASE("Encryption Test") {
                const auto customSealOperations = GetUnique<CustomSealOperations>();

                const auto schemeType = customSealOperations->GetSealOperations()->GetSealInfoPtr()->m_sealKeys.m_schemeType;
                bool isCKKS = schemeType == seal::scheme_type::ckks;

                try {
                    if (isCKKS) {
                        EncryptionTest<double>(*customSealOperations, isCKKS);
                    } else {
                        EncryptionTest<std::uint64_t>(*customSealOperations, isCKKS);
                    }
                } catch (std::exception &exception) {
                    std::cout << exception.what() << std::endl;
                }
            }

            SUBCASE("Serialization Test") {
                const auto customSealOperations = GetUnique<CustomSealOperations>();
                bool isCKKS = customSealOperations->GetSealOperations()->GetSealInfoPtr()->m_sealKeys.m_schemeType ==
                              seal::scheme_type::ckks;
                const auto num = isCKKS ? GetRandomNumberVariant<double>() : GetRandomNumberVariant<std::uint64_t>();

                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = GetUnique<std::stringstream>(*cipherStringPtr);

                const auto newCipher = customSealOperations->GetCipherFromBuffer(stream);
                const auto &variant = customSealOperations->Decrypt(*newCipher);

                const auto decNum = GetAnyVariant<double>(&variant);

                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&std::get<double>(num), &decNum));
                } else {
                    CHECK(std::get<std::uint64_t>(num) == decNum);
                }
            }

            SUBCASE("Serialization Test 2") {
                const auto customSealOperations = GetUnique<CustomSealOperations>();
                bool isCKKS = customSealOperations->GetSealOperations()->GetSealInfoPtr()->m_sealKeys.m_schemeType ==
                              seal::scheme_type::ckks;
                const auto num = isCKKS ? GetRandomNumberVariant<double>() : GetRandomNumberVariant<std::uint64_t>();

                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                const auto &variant = customSealOperations->DecryptFromBuffer(stream);

                if (isCKKS) {
                    const auto decNum = std::get<double>(variant);
                    CHECK(CompareWithDecimalTolerance(&std::get<double>(num), &decNum));
                } else {
                    const auto decNum = std::get<std::uint64_t>(variant);
                    CHECK(std::get<std::uint64_t>(num) == decNum);
                }
            }

            SUBCASE("Serialization With CKKS Test") {

                SealKeys keys{};
                keys.m_schemeType = seal::scheme_type::ckks;
                keys.m_isEncodingEnabled = true;

                const auto customSealOperations = GetUnique<CustomSealOperations>(keys);

                const auto num = GetRandomNumber<double>();

                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                const auto &variant = customSealOperations->DecryptFromBuffer(stream);

                const auto decNum = std::get<double>(variant);
                CHECK(CompareWithDecimalTolerance(&num, &decNum));

                //symmetric
                const auto &symmetricBuffer = customSealOperations->GetSymmetricEncryptedBuffer(num);
                const auto symmetricStream = GetUniqueStream(*symmetricBuffer);
                const auto &symmetricVariant = customSealOperations->DecryptFromBuffer(symmetricStream);

                const auto symmetricDecNum = std::get<double>(symmetricVariant);
                CHECK(CompareWithDecimalTolerance(&num, &symmetricDecNum));
            }

            SUBCASE("Serialization With Encoded BFV Test") {

                SealKeys keys{};
                keys.m_schemeType = seal::scheme_type::bfv;
                keys.m_isEncodingEnabled = true;

                const auto customSealOperations = GetUnique<CustomSealOperations>(keys);

                const auto num = GetRandomNumber<std::uint64_t>();

                // asymmetric
                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                const auto &variant = customSealOperations->DecryptFromBuffer(stream);

                const auto decNum = std::get<std::uint64_t>(variant);
                CHECK(num == decNum);

                //symmetric
                const auto &symmetricBuffer = customSealOperations->GetSymmetricEncryptedBuffer(num);
                const auto symmetricStream = GetUniqueStream(*symmetricBuffer);
                const auto &symmetricVariant = customSealOperations->DecryptFromBuffer(symmetricStream);

                const auto symmetricDecNum = std::get<std::uint64_t>(symmetricVariant);
                CHECK(num == symmetricDecNum);
            }

            SUBCASE("Serialization With Unencoded BFV Test") {

                SealKeys keys{};
                keys.m_schemeType = seal::scheme_type::bfv;
                keys.m_isEncodingEnabled = false;

                const auto customSealOperations = GetUnique<CustomSealOperations>(keys);

                const auto num = GetRandomNumber<std::uint64_t>();

                // asymmetric
                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                const auto &variant = customSealOperations->DecryptFromBuffer(stream);

                const auto decNum = std::get<std::uint64_t>(variant);
                CHECK(num == decNum);

                //symmetric
                const auto &symmetricBuffer = customSealOperations->GetSymmetricEncryptedBuffer(num);
                const auto symmetricStream = GetUniqueStream(*symmetricBuffer);
                const auto &symmetricVariant = customSealOperations->DecryptFromBuffer(symmetricStream);

                const auto symmetricDecNum = std::get<std::uint64_t>(symmetricVariant);
                CHECK(num == symmetricDecNum);
            }

            SUBCASE("Serialization With Encoded BGV Test") {

                SealKeys keys{};
                keys.m_schemeType = seal::scheme_type::bgv;
                keys.m_isEncodingEnabled = true;

                const auto customSealOperations = GetUnique<CustomSealOperations>(keys);

                const auto num = GetRandomNumber<std::uint64_t>();

                // asymmetric
                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                const auto &variant = customSealOperations->DecryptFromBuffer(stream);

                const auto decNum = std::get<std::uint64_t>(variant);
                CHECK(num == decNum);

                //symmetric
                const auto &symmetricBuffer = customSealOperations->GetSymmetricEncryptedBuffer(num);
                const auto symmetricStream = GetUniqueStream(*symmetricBuffer);
                const auto &symmetricVariant = customSealOperations->DecryptFromBuffer(symmetricStream);

                const auto symmetricDecNum = std::get<std::uint64_t>(symmetricVariant);
                CHECK(num == symmetricDecNum);
            }

            SUBCASE("Serialization With Unencoded BGV Test") {

                SealKeys keys{};
                keys.m_schemeType = seal::scheme_type::bgv;
                keys.m_isEncodingEnabled = false;

                const auto customSealOperations = GetUnique<CustomSealOperations>(keys);

                const auto num = GetRandomNumber<std::uint64_t>();

                // asymmetric
                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = std::make_unique<std::stringstream>(*cipherStringPtr);
                const auto &variant = customSealOperations->DecryptFromBuffer(stream);

                const auto decNum = std::get<std::uint64_t>(variant);
                CHECK(num == decNum);

                //symmetric
                const auto &symmetricBuffer = customSealOperations->GetSymmetricEncryptedBuffer(num);
                const auto symmetricStream = GetUniqueStream(*symmetricBuffer);
                const auto &symmetricVariant = customSealOperations->DecryptFromBuffer(symmetricStream);

                const auto symmetricDecNum = std::get<std::uint64_t>(symmetricVariant);
                CHECK(num == symmetricDecNum);
            }

            SUBCASE("NumVariantToAny Test") {
                const auto customSealOperations = GetUnique<CustomSealOperations>();
                bool isCKKS = customSealOperations->GetSealOperations()->GetSealInfoPtr()->m_sealKeys.m_schemeType ==
                              seal::scheme_type::ckks;
                const auto num = isCKKS ? GetRandomNumberVariant<double>() : GetRandomNumberVariant<std::uint64_t>();

                const auto cipherStringPtr = customSealOperations->GetEncryptedBuffer(num);
                const auto stream = GetUnique<std::stringstream>(*cipherStringPtr);

                const auto newCipher = customSealOperations->GetCipherFromBuffer(stream);
                const auto &variant = customSealOperations->Decrypt(*newCipher);

                const auto decNum = GetAnyVariant<double>(&variant);

                if (isCKKS) {
                    CHECK(CompareWithDecimalTolerance(&std::get<double>(num), &decNum));
                } else {
                    CHECK(std::get<std::uint64_t>(num) == decNum);
                }
            }

            SUBCASE("SealKeys Initialization test") {
                SealKeys keys{};
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
