
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
#include "ApplicationConstants.h"

namespace yakbas::sec::test {

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
                const auto customSealOperations = util::GetUnique<CustomSealOperations>();
                std::uint64_t num = util::GetRandomNumber();
                CHECK(num <= constants::APP_MAX_RANDOM_NUMBER);

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
                SealKeys keys{};
                CHECK(keys.m_schemeType == static_cast<const seal::scheme_type>(SEAL_SCHEME_TYPE));
                CHECK(keys.m_polyModulusDegree == SEAL_POLY_MODULUS_DEGREE);
                CHECK(keys.m_plainModulus == SEAL_PLAIN_MODULUS_DEGREE);
            }

            ::log4cplus::deinitialize();
        }

        TEST_CASE("Encoder Tests") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            using namespace seal;

            EncryptionParameters params(scheme_type::ckks);
            const std::vector<int> bit_sizes{60, 40, 40, 40, 60};

            size_t poly_modulus_degree = 16384;
            params.set_poly_modulus_degree(poly_modulus_degree);
            params.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, bit_sizes));

            SEALContext context(params);

            KeyGenerator keyGen(context);
            PublicKey pub;
            keyGen.create_public_key(pub);
            const SecretKey &sec = keyGen.secret_key();

            Encryptor enc(context, pub);
            Decryptor dec(context, sec);

            double scale = pow(2.0, 40);
            CKKSEncoder encoder(context);

            for (int i = 0; i < 2; ++i) {
                double unit_price = 3.24;
                double coeff = 1.28;
                unit_price += static_cast<double>(util::GetRandomNumber());
                coeff += static_cast<double>(util::GetRandomNumber());
                Plaintext plain_unit_price, plain_coeff;
                encoder.encode(unit_price, scale, plain_unit_price);
                encoder.encode(coeff, scale, plain_coeff);
                auto qualifiers = context.first_context_data()->qualifiers();
                LOG4CPLUS_INFO(*logger, "CKKS Batching: " + std::to_string(qualifiers.using_batching));
                Ciphertext cipher_unit_price, cipher_coeff;
                enc.encrypt(plain_unit_price, cipher_unit_price);
                enc.encrypt(plain_coeff, cipher_coeff);

                Evaluator evaluator(context);
                evaluator.multiply_inplace(cipher_unit_price, cipher_coeff);
                evaluator.multiply_inplace(cipher_unit_price, cipher_coeff);
                Plaintext decryptPlain;
                dec.decrypt(cipher_unit_price, decryptPlain);
                std::vector<double> output;
                encoder.decode(decryptPlain, output);
                double plainResult = unit_price * coeff * coeff;
                double val = output.at(0);

                CHECK(util::CompareWithTolerance(&plainResult, &val));
            }

            ::log4cplus::deinitialize();
        }

        TEST_CASE("Encoder Tests 2") {

            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            using namespace seal;
            using namespace std;

            EncryptionParameters parms(scheme_type::bgv);
            size_t poly_modulus_degree = 8192;
            parms.set_poly_modulus_degree(poly_modulus_degree);
            parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
            parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

            SEALContext context(parms);

            KeyGenerator keygen(context);
            SecretKey secret_key = keygen.secret_key();
            PublicKey public_key;
            keygen.create_public_key(public_key);
            RelinKeys relin_keys;
            keygen.create_relin_keys(relin_keys);
            Encryptor encryptor(context, public_key);
            Evaluator evaluator(context);
            Decryptor decryptor(context, secret_key);

            BatchEncoder batch_encoder(context);

            size_t slot_count = batch_encoder.slot_count();
            size_t row_size = slot_count / 2;
            cout << "Plaintext matrix row size: " << row_size << endl;

            for (int i = 0; i < 2; ++i) {
                try {
                    auto qualifiers = context.first_context_data()->qualifiers();
                    LOG4CPLUS_INFO(*logger, "BFV/BGV Batching: " + std::to_string(qualifiers.using_batching));

                    vector<uint64_t> unit_price(slot_count, 0ULL);
                    unit_price[0] = util::GetRandomNumber();

                    vector<uint64_t> coeff(slot_count, 0ULL);
                    coeff[0] = util::GetRandomNumber();

                    vector<uint64_t> discount(slot_count, 0ULL);
                    discount[0] = util::GetRandomNumber() % 3;

                    Plaintext plain_unit_price, plain_coeff, plain_discount;

                    batch_encoder.encode(unit_price, plain_unit_price);
                    batch_encoder.encode(coeff, plain_coeff);
                    batch_encoder.encode(discount, plain_discount);

                    Ciphertext cipher_unit_price, cipher_coeff, cipher_discount;

                    encryptor.encrypt(plain_unit_price, cipher_unit_price);
                    encryptor.encrypt(plain_coeff, cipher_coeff);
                    encryptor.encrypt(plain_discount, cipher_discount);

                    evaluator.multiply_inplace(cipher_unit_price, cipher_coeff);
                    evaluator.multiply_inplace(cipher_unit_price, cipher_coeff);
                    evaluator.sub_inplace(cipher_unit_price, cipher_discount);

                    auto budget = decryptor.invariant_noise_budget(cipher_unit_price);

                    if (budget == 0) {
                        cerr << "Budget is zero" << endl;
                    }

                    Plaintext decryptedPlain;
                    decryptor.decrypt(cipher_unit_price, decryptedPlain);

                    vector<uint64_t> output;
                    batch_encoder.decode(decryptedPlain, output);

                    LOG4CPLUS_INFO(*logger, "Unit Price: " + std::to_string(unit_price.at(0)));
                    LOG4CPLUS_INFO(*logger, "coeff: " + std::to_string(coeff.at(0)));
                    LOG4CPLUS_INFO(*logger, "Discount: " + std::to_string(discount.at(0)));

                    const auto result = (unit_price.at(0) * coeff.at(0)) * coeff.at(0) - discount.at(0);
                    LOG4CPLUS_INFO(*logger, "Result: " + std::to_string(result));
                    const std::uint64_t &at = output.at(0);
                    LOG4CPLUS_INFO(*logger, "Output: " + std::to_string(at));
                    CHECK(result == at);
                } catch (const std::exception &e) {
                    LOG4CPLUS_ERROR(*logger,
                                    std::string("exception message: ") + e.what());
                }
            }
            ::log4cplus::deinitialize();
        }

        TEST_CASE("Is Batching") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            using namespace seal;
            using namespace std;


            EncryptionParameters params(scheme_type::ckks);
            const std::vector<int> bit_sizes{60, 40, 40, 40, 60};
            params.set_poly_modulus_degree(SEAL_POLY_MODULUS_DEGREE);
            params.set_coeff_modulus(CoeffModulus::Create(SEAL_POLY_MODULUS_DEGREE, bit_sizes));

            SEALContext context(params);

            const auto &firstContextData = context.first_context_data();
            auto qualifiers = firstContextData->qualifiers().using_batching;
            LOG4CPLUS_INFO(*logger, "BFV/BGV Batching: " + std::to_string(qualifiers));

            ::log4cplus::deinitialize();
        }

    }

} // yakbas

#endif

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#undef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
