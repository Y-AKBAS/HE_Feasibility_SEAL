
#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif

#include "doctest/doctest.h"

#if (!DISABLE_TESTS)

#include "CustomSealOperations.h"
#include "Timer.h"
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
                    LOG4CPLUS_INFO(*logger, "Encryption Test Exception Message: "s + exception.what());
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
                CHECK(keys.m_isEncodingEnabled == ENCODING_ENABLED);
                CHECK(keys.m_scalePower == CKKS_SCALE_POWER);

                for (int i = 0; i < constants::ENCODING_BIT_SIZES.size(); ++i) {
                    CHECK(constants::ENCODING_BIT_SIZES.at(i) == keys.m_encodingBitSizes.at(i));
                }
            }

            ::log4cplus::deinitialize();
        }

        TEST_CASE("ProcessedCipher Addition Test For CKKS") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            try {
                SealKeys keys{};
                keys.m_schemeType = seal::scheme_type::ckks;
                keys.m_isEncodingEnabled = true;

                const auto customSealOperations = GetUnique<CustomSealOperations>(keys);
                const auto variant = GetRandomNumberVariant<double>();
                const auto num = GetAnyVariant<double>(&variant);
                const auto expectedResult = num * num + num - num;

                LOG4CPLUS_INFO(*logger, "random number: "s + std::to_string(num));
                LOG4CPLUS_INFO(*logger, "expectedResult: "s + std::to_string(expectedResult));

                const auto &bufferString = customSealOperations->GetSymmetricEncryptedBuffer(variant);

                auto coeffCipher = customSealOperations->GetCipherFromBuffer(GetUniqueStream(*bufferString));
                auto unitPriceCipher = customSealOperations->GetCipherFromBuffer(GetUniqueStream(*bufferString));
                auto discountCipher = customSealOperations->GetCipherFromBuffer(GetUniqueStream(*bufferString));
                auto seatPriceCipher = customSealOperations->GetCipherFromBuffer(GetUniqueStream(*bufferString));

                auto firstTotalCipher = customSealOperations->GetSealOperations()->GetNewCipher(
                        std::make_optional(coeffCipher->parms_id()));

                customSealOperations->GetEvaluatorPtr()->multiply(*coeffCipher, *unitPriceCipher,
                                                                  *firstTotalCipher);
                customSealOperations->GetSealOperations()->SubProcessedInPlace(*firstTotalCipher, *discountCipher);

                customSealOperations->AddProcessedInPlace(*firstTotalCipher, *seatPriceCipher);

                const std::string &firstTotalBuffer = CustomSealOperations::GetBufferFromCipher(*firstTotalCipher);
                const num_variant &firstTotalVariant = customSealOperations->DecryptFromBuffer(
                        GetUniqueStream(firstTotalBuffer));
                const auto &firstTotalNum = GetAnyVariant<double>(&firstTotalVariant);

                CHECK(CompareWithDecimalTolerance(&expectedResult, &firstTotalNum, 3));

                auto secondTotalCipher = customSealOperations->GetSealOperations()->GetNewCipher(
                        std::make_optional(coeffCipher->parms_id()));

                customSealOperations->GetEvaluatorPtr()->multiply(*coeffCipher, *unitPriceCipher,
                                                                  *secondTotalCipher);
                customSealOperations->GetSealOperations()->SubProcessedInPlace(*secondTotalCipher, *discountCipher);

                customSealOperations->AddProcessedInPlace(*secondTotalCipher, *seatPriceCipher);

                const std::string &secondTotalBuffer = CustomSealOperations::GetBufferFromCipher(*secondTotalCipher);
                const num_variant &secondTotalVariant = customSealOperations->DecryptFromBuffer(
                        GetUniqueStream(secondTotalBuffer));
                const auto &secondTotalNum = GetAnyVariant<double>(&secondTotalVariant);

                CHECK(CompareWithDecimalTolerance(&expectedResult, &secondTotalNum, 3));

                customSealOperations->AddProcessedInPlace(*firstTotalCipher, *secondTotalCipher);
                const auto &finalVariant = customSealOperations->Decrypt(*firstTotalCipher);

                const auto &finalTotalNum = GetAnyVariant<double>(&finalVariant);

                const double finalExpectedResult = expectedResult + expectedResult;
                CHECK(CompareWithDecimalTolerance(&finalExpectedResult, &finalTotalNum, 3));

            } catch (std::exception &e) {
                LOG4CPLUS_ERROR(*logger, "exception message: "s + e.what());
            }
            log4cplus::deinitialize();
        }
    }

    TEST_CASE("Customization Tests") {
        ::log4cplus::initialize();
        ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);

        SUBCASE("CKKS Customization Test") {
            SealKeys keys{};
            keys.m_schemeType = seal::scheme_type::ckks;
            keys.m_isEncodingEnabled = true;

            const auto encryptedPlain = GetRandomNumber<double>();
            const auto firstCipher = GetUnique<CustomSealOperations>(keys)->Encrypt(encryptedPlain);

            const num_variant &decryptedCipher = GetUnique<CustomSealOperations>(keys)->Decrypt(*firstCipher);

            auto decryptedPlain = GetAnyVariant<double>(&decryptedCipher);

            CHECK(CompareWithDecimalTolerance(&encryptedPlain, &decryptedPlain) == false);
        }

        SUBCASE("BFV Customization Test") {
            SealKeys keys{};
            keys.m_schemeType = seal::scheme_type::bfv;
            keys.m_isEncodingEnabled = true;

            const auto encryptedPlain = GetRandomNumber<std::uint64_t>();
            const auto firstCipher = GetUnique<CustomSealOperations>(keys)->Encrypt(encryptedPlain);

            const num_variant &decryptedCipher = GetUnique<CustomSealOperations>(keys)->Decrypt(*firstCipher);

            auto decryptedPlain = GetAnyVariant<std::uint64_t>(&decryptedCipher);

            CHECK(encryptedPlain != decryptedPlain);
        }

        SUBCASE("BGV Customization Test") {
            SealKeys keys{};
            keys.m_schemeType = seal::scheme_type::bgv;
            keys.m_isEncodingEnabled = true;

            const auto encryptedPlain = GetRandomNumber<std::uint64_t>();
            const auto firstCipher = GetUnique<CustomSealOperations>(keys)->Encrypt(encryptedPlain);

            const num_variant &decryptedCipher = GetUnique<CustomSealOperations>(keys)->Decrypt(*firstCipher);

            auto decryptedPlain = GetAnyVariant<std::uint64_t>(&decryptedCipher);

            CHECK(encryptedPlain != decryptedPlain);
        }

        log4cplus::deinitialize();
    }

    TEST_CASE("Dummy") {
        ::log4cplus::initialize();
        ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
        const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

        try {

            SealKeys sealKeys{};
            sealKeys.m_isEncodingEnabled = true;
            sealKeys.m_schemeType = seal::scheme_type::ckks;

            const auto sealOperations = GetUnique<CustomSealOperations>(sealKeys);

            std::uint64_t startMillis = Timer::GetCurrentTimeMillis();
            auto startCipherPtr = sealOperations->Encrypt(startMillis);

            std::this_thread::sleep_for(std::chrono::seconds(1));

            const auto unitPrice = GetRandomNumberVariant<double>();
            std::uint64_t endMillis = Timer::GetCurrentTimeMillis();
            auto priceCipher = sealOperations->Encrypt(unitPrice);
            auto endCipher = sealOperations->Encrypt(endMillis);

            sealOperations->GetEvaluatorPtr()->sub_inplace(*endCipher, *startCipherPtr);
            sealOperations->GetEvaluatorPtr()->multiply_inplace(*endCipher, *priceCipher);

            auto resultVar = sealOperations->Decrypt(*endCipher);
            auto cipherResult = GetAnyVariant<double>(&resultVar);

            auto plainResult = (endMillis - startMillis) * GetAnyVariant<double>(&unitPrice);

            CHECK(CompareWithDecimalTolerance(&cipherResult, &plainResult));

        } catch(std::exception &e) {
            LOG4CPLUS_ERROR(*logger, e.what());
        }

        ::log4cplus::deinitialize();
    }

} // yakbas

#endif

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#undef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
