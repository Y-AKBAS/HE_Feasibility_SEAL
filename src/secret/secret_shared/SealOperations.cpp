
#include "SealOperations.h"
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    SealOperations::SealOperations(const SealKeys &sealKeys)
            : m_sealInfoPtr(std::make_unique<SealInfo>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("SealOperations"))),
              m_schemeType(sealKeys.m_schemeType),
              m_scale(sealKeys.m_scale),
              m_scalePower(sealKeys.m_scalePower) {

        if (m_sealInfoPtr->m_sealKeys.m_isEncodingEnabled) {

            if (m_schemeType == seal::scheme_type::bgv ||
                m_schemeType == seal::scheme_type::bfv) {
                m_batchEncoder = std::make_unique<seal::BatchEncoder>(*m_sealInfoPtr->m_sealContextPtr);
            } else {
                m_ckksEncoder = std::make_unique<seal::CKKSEncoder>(*m_sealInfoPtr->m_sealContextPtr);
            }

        }
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::Encrypt(const num_variant &num, const seal::Encryptor &encryptor) const {

        if (m_sealInfoPtr->m_sealKeys.m_isEncodingEnabled) {
            return this->EncodeAndEncrypt(num, encryptor);
        }

        if (const auto value = std::get_if<std::uint64_t>(&num)) {
            const std::string hexString = seal::util::uint_to_hex_string(value, std::size_t(1));
            seal::Plaintext plaintext(hexString);
            auto cipherText = GetUnique<seal::Ciphertext>();
            encryptor.encrypt(plaintext, *cipherText);
            return cipherText;
        }

        throw std::invalid_argument("Type mismatch! It should be std::uint64_t for encryption");
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::EncodeAndEncrypt(const num_variant &num,
                                     const seal::Encryptor &encryptor) const {

        auto cipherText = GetUnique<seal::Ciphertext>();
        seal::Plaintext plaintext;
        if (m_schemeType == seal::scheme_type::ckks) {
            m_ckksEncoder->encode(std::get<double>(num), m_scale, plaintext);
            encryptor.encrypt(plaintext, *cipherText);
            return cipherText;
        }

        std::vector<std::uint64_t> vec(m_batchEncoder->slot_count(), 0ULL);
        vec.at(0) = std::get<std::uint64_t>(num);
        m_batchEncoder->encode(vec, plaintext);
        encryptor.encrypt(plaintext, *cipherText);

        return cipherText;
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::EncryptSymmetric(const num_variant &num, const seal::Encryptor &encryptor) const {

        if (m_sealInfoPtr->m_sealKeys.m_isEncodingEnabled) {
            return this->EncodeAndEncryptSymmetric(num, encryptor);
        }

        const std::string hexString = seal::util::uint_to_hex_string(&std::get<std::uint64_t>(num), std::size_t(1));
        const auto plainTextToEncrypt = GetUnique<seal::Plaintext>(hexString);
        auto cipherText = GetUnique<seal::Ciphertext>();
        encryptor.encrypt_symmetric(*plainTextToEncrypt, *cipherText);
        return cipherText;
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::EncodeAndEncryptSymmetric(const num_variant &num,
                                              const seal::Encryptor &encryptor) const {

        auto cipherText = GetUnique<seal::Ciphertext>();
        seal::Plaintext plaintext;
        if (m_schemeType == seal::scheme_type::ckks) {
            m_ckksEncoder->encode(std::get<double>(num), m_scale, plaintext);
            encryptor.encrypt_symmetric(plaintext, *cipherText);
            return cipherText;
        }

        std::vector<std::uint64_t> vec(m_batchEncoder->slot_count(), 0ULL);
        vec.at(0) = std::get<std::uint64_t>(num);
        m_batchEncoder->encode(vec, plaintext);
        encryptor.encrypt_symmetric(plaintext, *cipherText);

        return cipherText;
    }

    num_variant SealOperations::Decrypt(const seal::Ciphertext &cipher,
                                        seal::Decryptor &decryptor) const {

        if (m_sealInfoPtr->m_sealKeys.m_isEncodingEnabled) {
            return this->DecodeAndDecrypt(cipher, decryptor);
        }

        const auto decryptedPlaintext = GetUnique<seal::Plaintext>();
        decryptor.decrypt(cipher, *decryptedPlaintext);
        const std::string hexString = decryptedPlaintext->to_string();
        std::uint64_t result{};
        seal::util::hex_string_to_uint(hexString.c_str(), static_cast<int>(hexString.length()),
                                       std::size_t(1), &result);
        return result;
    }

    num_variant SealOperations::DecodeAndDecrypt(const seal::Ciphertext &cipher,
                                                 seal::Decryptor &decryptor) const {

        try {
            seal::Plaintext decryptedPlain;
            decryptor.decrypt(cipher, decryptedPlain);

            if (m_schemeType == seal::scheme_type::ckks) {
                std::vector<double> output;
                m_ckksEncoder->decode(decryptedPlain, output);
                return output.at(0);
            } else {
                std::vector<std::uint64_t> output;
                m_batchEncoder->decode(decryptedPlain, output);
                return output.at(0);
            }
        } catch (const std::exception &exception) {
            LOG4CPLUS_ERROR(*m_logger, "Decryption exception message: "s + exception.what());
            throw exception;
        }
    }

    std::unique_ptr<std::string> SealOperations::GetEncryptedBuffer(const std::variant<std::uint64_t, double, int> &num,
                                                                    const seal::Encryptor &encryptor) const {
        const auto &stream = GetUniqueStream();
        const auto &cipherPtr = this->Encrypt(num, encryptor);
        cipherPtr->save(*stream);
        return std::make_unique<std::string>(stream->str());
    }

    std::unique_ptr<std::string> SealOperations::GetSymmetricEncryptedBuffer(const num_variant &num,
                                                                             const seal::Encryptor &encryptor) const {
        const auto &stream = GetUniqueStream();
        const auto &cipherPtr = this->EncryptSymmetric(num, encryptor);
        cipherPtr->save(*stream);
        return std::make_unique<std::string>(stream->str());
    }

    void SealOperations::AddProcessedInPlace(seal::Ciphertext &processedCipher,
                                             seal::Ciphertext &cipherToAdd) const {
        try {
            if (m_schemeType != seal::scheme_type::ckks) {
                m_sealInfoPtr->m_evaluatorPtr->add_inplace(processedCipher, cipherToAdd);
                return;
            }

            double processedCipherScale = std::log2(processedCipher.scale());
            double cipherToAddScale = std::log2(cipherToAdd.scale());

            LOG4CPLUS_INFO(*m_logger, "processedCipher scale: " + std::to_string(processedCipherScale));
            LOG4CPLUS_INFO(*m_logger, "cipherToAdd scale: " + std::to_string(cipherToAddScale));

            if (processedCipherScale > m_scalePower) {
                m_sealInfoPtr->m_evaluatorPtr->rescale_to_next_inplace(processedCipher);
            }

            if (cipherToAddScale > m_scalePower) {
                m_sealInfoPtr->m_evaluatorPtr->rescale_to_next_inplace(cipherToAdd);
            }

            LOG4CPLUS_INFO(*m_logger, "After rescaling processedCipher scale: " +
                                      std::to_string(std::log2(processedCipher.scale())));
            LOG4CPLUS_INFO(*m_logger,
                           "After rescaling cipherToAdd scale: " + std::to_string(std::log2(cipherToAdd.scale())));

            if (CompareWithTolerance(&processedCipherScale, &cipherToAddScale, 5)) {
                processedCipher.scale() = cipherToAdd.scale();
                LOG4CPLUS_INFO(*m_logger,
                               "Cipher scales are being equal now. Scale: " + std::to_string(processedCipherScale));
            }

            m_sealInfoPtr->m_evaluatorPtr->mod_switch_to_inplace(cipherToAdd, processedCipher.parms_id());
            m_sealInfoPtr->m_evaluatorPtr->add_inplace(processedCipher, cipherToAdd);
        } catch (const std::exception &exception) {
            LOG4CPLUS_ERROR(*m_logger,
                            std::string("Exception thrown in AddProcessedInPlace. Message: ") + exception.what());
        }
    }

    void SealOperations::SubProcessedInPlace(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd) const {
        try {
            if (m_schemeType != seal::scheme_type::ckks) {
                m_sealInfoPtr->m_evaluatorPtr->sub_inplace(processedCipher, cipherToAdd);
                return;
            }

            double processedCipherScale = std::log2(processedCipher.scale());
            double cipherToAddScale = std::log2(cipherToAdd.scale());

            LOG4CPLUS_INFO(*m_logger, "processedCipher scale: " + std::to_string(processedCipherScale));
            LOG4CPLUS_INFO(*m_logger, "cipherToAdd scale: " + std::to_string(cipherToAddScale));

            if (processedCipherScale > m_scalePower) {
                m_sealInfoPtr->m_evaluatorPtr->rescale_to_next_inplace(processedCipher);
            }

            if (cipherToAddScale > m_scalePower) {
                m_sealInfoPtr->m_evaluatorPtr->rescale_to_next_inplace(cipherToAdd);
            }

            LOG4CPLUS_INFO(*m_logger, "After rescaling processedCipher scale: " +
                                      std::to_string(std::log2(processedCipher.scale())));
            LOG4CPLUS_INFO(*m_logger,
                           "After rescaling cipherToAdd scale: " + std::to_string(std::log2(cipherToAdd.scale())));

            if (CompareWithTolerance(&processedCipherScale, &cipherToAddScale, 5)) {
                LOG4CPLUS_INFO(*m_logger,
                               "Cipher scales are being equal now. Scale: " + std::to_string(processedCipherScale));
                processedCipher.scale() = cipherToAdd.scale();
            }

            m_sealInfoPtr->m_evaluatorPtr->mod_switch_to_inplace(cipherToAdd, processedCipher.parms_id());
            m_sealInfoPtr->m_evaluatorPtr->sub_inplace(processedCipher, cipherToAdd);
        } catch (const std::exception &exception) {
            LOG4CPLUS_ERROR(*m_logger,
                            std::string("Exception thrown in SubProcessedInPlace. Message: ") + exception.what());
        }
    }

    void SealOperations::AddProcessedCiphers(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd,
                                             seal::Ciphertext &destination, const seal::Evaluator &evaluator) const {
        try {
            if (m_schemeType != seal::scheme_type::ckks) {
                evaluator.add_inplace(processedCipher, cipherToAdd);
                return;
            }

            double processedCipherScale = std::log2(processedCipher.scale());
            double cipherToAddScale = std::log2(cipherToAdd.scale());

            if (processedCipherScale > m_scalePower) {
                evaluator.rescale_to_next_inplace(processedCipher);
            }

            if (cipherToAddScale > m_scalePower) {
                evaluator.rescale_to_next_inplace(cipherToAdd);
            }

            LOG4CPLUS_INFO(*m_logger, "processedCipher scale: " + std::to_string(processedCipherScale));
            LOG4CPLUS_INFO(*m_logger, "cipherToAdd scale: " + std::to_string(cipherToAddScale));
            if (CompareWithTolerance(&processedCipherScale, &cipherToAddScale, 4)) {
                processedCipher.scale() = cipherToAdd.scale();
                LOG4CPLUS_INFO(*m_logger,
                               "Cipher scales are being equal now. Scale: " + std::to_string(processedCipherScale));
            }

            evaluator.mod_switch_to_inplace(cipherToAdd, processedCipher.parms_id());
            evaluator.add_inplace(processedCipher, cipherToAdd);
        } catch (const std::exception &exception) {
            LOG4CPLUS_ERROR(*m_logger,
                            std::string("Exception thrown in AddProcessedInPlace. Message: ") + exception.what());
        }
    }

    void SealOperations::SubProcessedCiphers(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd,
                                             seal::Ciphertext &destination, const seal::Evaluator &evaluator) const {

        try {
            if (m_schemeType != seal::scheme_type::ckks) {
                evaluator.sub_inplace(processedCipher, cipherToAdd);
                return;
            }

            evaluator.rescale_to_next_inplace(processedCipher);
            double processedCipherScale = std::log2(processedCipher.scale());
            double cipherToAddScale = std::log2(cipherToAdd.scale());
            LOG4CPLUS_INFO(*m_logger, "processedCipher scale: " + std::to_string(processedCipherScale));
            LOG4CPLUS_INFO(*m_logger, "cipherToAdd scale: " + std::to_string(cipherToAddScale));
            if (CompareWithTolerance(&processedCipherScale, &cipherToAddScale, 5)) {
                LOG4CPLUS_INFO(*m_logger,
                               "Cipher scales are being equal now. Scale: " + std::to_string(processedCipherScale));
                processedCipher.scale() = cipherToAdd.scale();
            }

            evaluator.mod_switch_to_inplace(cipherToAdd, processedCipher.parms_id());
            evaluator.sub_inplace(processedCipher, cipherToAdd);
        } catch (const std::exception &exception) {
            LOG4CPLUS_ERROR(*m_logger,
                            std::string("Exception thrown in SubProcessedInPlace. Message: ") + exception.what());
        }
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::GetCipherFromBuffer(std::stringstream &stream) const {
        auto cipher = GetUnique<seal::Ciphertext>();
        try {
            cipher->load(*this->m_sealInfoPtr->m_sealContextPtr, stream);
        }
        catch (std::exception &e) {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error while GetCipherFromBuffer()... exp: " + std::string(e.what()));
        }
        return cipher;
    }

    num_variant SealOperations::DecryptFromBuffer(std::stringstream &stream,
                                                  seal::Decryptor &decryptor) const {
        const auto &cipherFromBuffer = this->GetCipherFromBuffer(stream);
        return this->Decrypt(*cipherFromBuffer, decryptor);
    }

    const std::unique_ptr<SealInfo> &SealOperations::GetSealInfoPtr() const {
        return m_sealInfoPtr;
    }

    std::unique_ptr<seal::PublicKey>
    SealOperations::GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const {
        auto keyPtr = GetUnique<seal::PublicKey>();
        keyPtr->load(*this->m_sealInfoPtr->m_sealContextPtr, *stream);
        return keyPtr;
    }

    std::unique_ptr<seal::RelinKeys>
    SealOperations::GetRelinKeysFromBuffer(const std::unique_ptr<std::stringstream> &stream) const {
        auto keyPtr = GetUnique<seal::RelinKeys>();
        keyPtr->load(*this->m_sealInfoPtr->m_sealContextPtr, *stream);
        return keyPtr;
    }

    bool SealOperations::operator==(const SealOperations &rhs) const {
        return this->m_sealInfoPtr->m_sealKeys == rhs.m_sealInfoPtr->m_sealKeys;
    }

    void SealOperations::Relinearize(seal::Ciphertext &ciphertext, const seal::RelinKeys &relinKeys) const {
        m_sealInfoPtr->m_evaluatorPtr->relinearize_inplace(ciphertext, relinKeys);
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::GetNewCipher(const std::optional<seal::parms_id_type> &parms_id) const {
        const static auto &sealContextPtr = m_sealInfoPtr->m_sealContextPtr;
        return parms_id.has_value() ? std::make_unique<seal::Ciphertext>(*sealContextPtr, parms_id.value())
                                    : std::make_unique<seal::Ciphertext>(*sealContextPtr);
    }

} // yakbas