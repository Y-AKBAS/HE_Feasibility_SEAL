#pragma once

#include <log4cplus/logger.h>
#include "SealOperations.h"

namespace yakbas::sec {

    class CustomSealOperations {
    public:

        explicit CustomSealOperations(const SealKeys &sealKeys = {});

        static const SealOperations &GetOperations(const SealKeys &sealKeys = {seal::scheme_type::bfv, 16384, 1024});

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> Encrypt(const std::uint64_t &num) const;

        [[nodiscard]] std::uint64_t Decrypt(const seal::Ciphertext &cipher) const;

        [[nodiscard]] std::unique_ptr<std::string> GetEncryptedBuffer(const uint64_t &num) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetCipherFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] std::unique_ptr<seal::PublicKey>
        GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] std::uint64_t DecryptFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] const SealOperations *GetSealOperations() const;

    private:
        const SealOperations *m_sealOperations{nullptr};
        std::unique_ptr<seal::PublicKey> m_publicKeyPtr{nullptr};
        std::unique_ptr<seal::SecretKey> m_secretKeyPtr{nullptr};
        std::unique_ptr<seal::Encryptor> m_encryptorPtr{nullptr};
        std::unique_ptr<seal::Decryptor> m_decryptorPtr{nullptr};
        std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

} // yakbas
