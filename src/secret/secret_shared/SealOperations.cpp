
#include "SealOperations.h"
#include "Utils.h"
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    SealOperations::SealOperations(const SealKeys &sealKeys)
            : m_sealInfoPtr(std::make_unique<SealInfo>(sealKeys)),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("SecretClientManager"))) {}

    std::unique_ptr<seal::Ciphertext>
    SealOperations::Encrypt(const uint64_t &num, const seal::Encryptor &encryptor) {
        const std::string hexString = seal::util::uint_to_hex_string(&num, std::size_t(1));
        const auto plainTextToEncrypt = GetUnique<seal::Plaintext>(hexString);
        auto cipherText = GetUnique<seal::Ciphertext>();
        encryptor.encrypt(*plainTextToEncrypt, *cipherText);
        return cipherText;
    }

    std::uint64_t SealOperations::Decrypt(const seal::Ciphertext &cipher,
                                          seal::Decryptor &decryptor) {

        const auto decryptedPlaintext = GetUnique<seal::Plaintext>();
        decryptor.decrypt(cipher, *decryptedPlaintext);
        const std::string hexString = decryptedPlaintext->to_string();
        std::uint64_t result{};
        seal::util::hex_string_to_uint(hexString.c_str(), static_cast<int>(hexString.length()),
                                       std::size_t(1), &result);
        return result;
    }

    std::unique_ptr<std::string> SealOperations::GetEncryptedBuffer(const uint64_t &num,
                                                                    const seal::Encryptor &encryptor) {
        const auto &stream = util::GetUniqueStream();
        SealOperations::Encrypt(num, encryptor)->save(*stream);
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