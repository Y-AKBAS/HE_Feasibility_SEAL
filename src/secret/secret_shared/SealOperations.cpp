
#include "SealOperations.h"
#include "Utils.h"
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    SealOperations::SealOperations(const SealKeys &sealKeys)
            : m_sealInfoPtr(std::make_unique<SealInfo>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("SealOperations"))),
              m_schemeType(sealKeys.m_schemeType) {

        if (m_schemeType == seal::scheme_type::bgv ||
            m_schemeType == seal::scheme_type::bfv) {
            m_batchEncoder = std::make_unique<seal::BatchEncoder>(*m_sealInfoPtr->m_sealContextPtr);
        } else {
            m_ckksEncoder = std::make_unique<seal::CKKSEncoder>(*m_sealInfoPtr->m_sealContextPtr);
        }
    }

    const double SealOperations::m_scale{std::pow(2.0, 40)};

    std::unique_ptr<seal::Ciphertext>
    SealOperations::Encrypt(const uint64_t &num, const seal::Encryptor &encryptor) const {

        if (m_sealInfoPtr->m_sealKeys.m_isEncodingEnabled) {
            return this->EncodeAndEncrypt(num, encryptor);
        }

        const std::string hexString = seal::util::uint_to_hex_string(&num, std::size_t(1));
        seal::Plaintext plaintext(hexString);
        auto cipherText = GetUnique<seal::Ciphertext>();
        encryptor.encrypt(plaintext, *cipherText);
        return cipherText;
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::EncodeAndEncrypt(const std::uint64_t &num,
                                     const seal::Encryptor &encryptor) const {

        auto cipherText = GetUnique<seal::Ciphertext>();
        seal::Plaintext plaintext;
        if (m_schemeType == seal::scheme_type::ckks) {
            m_ckksEncoder->encode(static_cast<double>(num), m_scale, plaintext);
            encryptor.encrypt(plaintext, *cipherText);
            return cipherText;
        }

        std::vector<std::uint64_t> vec(m_batchEncoder->slot_count(), 0ULL);
        vec.at(0) = num;
        m_batchEncoder->encode(vec, plaintext);
        encryptor.encrypt(plaintext, *cipherText);

        return cipherText;
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::EncryptSymmetric(const uint64_t &num, const seal::Encryptor &encryptor) const {

        if (m_sealInfoPtr->m_sealKeys.m_isEncodingEnabled) {
            return this->EncodeAndEncryptSymmetric(num, encryptor);
        }

        const std::string hexString = seal::util::uint_to_hex_string(&num, std::size_t(1));
        const auto plainTextToEncrypt = GetUnique<seal::Plaintext>(hexString);
        auto cipherText = GetUnique<seal::Ciphertext>();
        encryptor.encrypt_symmetric(*plainTextToEncrypt, *cipherText);
        return cipherText;
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::EncodeAndEncryptSymmetric(const std::uint64_t &num,
                                              const seal::Encryptor &encryptor) const {

        auto cipherText = GetUnique<seal::Ciphertext>();
        seal::Plaintext plaintext;
        if (m_schemeType == seal::scheme_type::ckks) {
            m_ckksEncoder->encode(static_cast<double>(num), m_scale, plaintext);
            encryptor.encrypt_symmetric(plaintext, *cipherText);
            return cipherText;
        }

        std::vector<std::uint64_t> vec(m_batchEncoder->slot_count(), 0ULL);
        vec.at(0) = num;
        m_batchEncoder->encode(vec, plaintext);
        encryptor.encrypt_symmetric(plaintext, *cipherText);

        return cipherText;
    }

    std::uint64_t SealOperations::Decrypt(const seal::Ciphertext &cipher,
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

    std::uint64_t SealOperations::DecodeAndDecrypt(const seal::Ciphertext &cipher,
                                                   seal::Decryptor &decryptor) const {

        seal::Plaintext decryptedPlain;
        decryptor.decrypt(cipher, decryptedPlain);

        if (m_schemeType == seal::scheme_type::ckks) {
            std::vector<double> output;
            m_ckksEncoder->decode(decryptedPlain, output);
            return static_cast<std::uint64_t>(output.at(0));
        } else {
            std::vector<uint64_t> output;
            m_batchEncoder->decode(decryptedPlain, output);
            return output.at(0);
        }
    }

    std::unique_ptr<std::string> SealOperations::GetEncryptedBuffer(const uint64_t &num,
                                                                    const seal::Encryptor &encryptor) const {
        const auto &stream = util::GetUniqueStream();
        const auto &cipherPtr = this->Encrypt(num, encryptor);
        cipherPtr->save(*stream);
        return std::make_unique<std::string>(stream->str());
    }

    std::unique_ptr<std::string> SealOperations::GetSymmetricEncryptedBuffer(const uint64_t &num,
                                                                             const seal::Encryptor &encryptor) const {
        const auto &stream = util::GetUniqueStream();
        const auto &cipherPtr = this->EncryptSymmetric(num, encryptor);
        cipherPtr->save(*stream);
        return std::make_unique<std::string>(stream->str());
    }

    std::unique_ptr<seal::Ciphertext>
    SealOperations::GetCipherFromBuffer(std::stringstream &stream) const {
        auto cipher = util::GetUnique<seal::Ciphertext>();
        try {
            cipher->load(*this->m_sealInfoPtr->m_sealContextPtr, stream);
        }
        catch (std::exception &e) {
            LOG4CPLUS_ERROR(*m_logger,
                            "Error while GetCipherFromBuffer()... excp: " + std::string(e.what()));
        }
        return cipher;
    }

    std::uint64_t SealOperations::DecryptFromBuffer(std::stringstream &stream,
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

    bool SealOperations::operator==(const SealOperations &rhs) const {
        return this->m_sealInfoPtr->m_sealKeys == rhs.m_sealInfoPtr->m_sealKeys;
    }

} // yakbas