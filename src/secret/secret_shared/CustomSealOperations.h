#pragma once

#include <log4cplus/logger.h>
#include "SealOperations.h"

namespace yakbas::sec {

    class CustomSealOperations {
    public:

        explicit CustomSealOperations(const SealKeys &sealKeys = {});

        static const SealOperations &GetOperations(const SealKeys &sealKeys = {});

        [[nodiscard]] static std::unique_ptr<seal::Encryptor>
        CreateNewEncryptor(const seal::PublicKey &publicKey, const SealKeys &sealKeys = {});

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> Encrypt(const std::uint64_t &num) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> EncryptSymmetric(const std::uint64_t &num) const;

        [[nodiscard]] std::uint64_t Decrypt(const seal::Ciphertext &cipher) const;

        [[nodiscard]] std::unique_ptr<std::string> GetEncryptedBuffer(const uint64_t &num) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetCipherFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] std::unique_ptr<seal::PublicKey>
        GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] std::uint64_t DecryptFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] const std::unique_ptr<seal::Encryptor> &GetEncryptorPtr() const;

        [[nodiscard]] const std::string &GetPublicKeyBuffer() const;

        [[nodiscard]] const SealOperations *GetSealOperations() const;

        [[nodiscard]] const std::unique_ptr<seal::Evaluator> &GetEvaluatorPtr() const;


        [[nodiscard]] const std::unique_ptr<seal::Decryptor> &GetDecryptorPtr() const;

        [[nodiscard]] static std::string GetBufferFromCipher(seal::Ciphertext &ciphertext);

        [[nodiscard]] std::unique_ptr<std::string> GetSymmetricEncryptedBuffer(const uint64_t &num) const;

        void Relinearize(seal::Ciphertext &ciphertext);

        void SwitchMode(seal::Ciphertext &ciphertext);

    private:

        [[nodiscard]] std::unique_ptr<std::stringstream> PublicKeyToBuffer() const;
        const SealOperations *m_sealOperations{nullptr};
        std::unique_ptr<seal::PublicKey> m_publicKeyPtr{nullptr};
        std::unique_ptr<seal::SecretKey> m_secretKeyPtr{nullptr};
        std::unique_ptr<seal::Encryptor> m_encryptorPtr{nullptr};
        std::unique_ptr<seal::Decryptor> m_decryptorPtr{nullptr};
        std::unique_ptr<seal::Evaluator> m_evaluatorPtr{nullptr};
        std::unique_ptr<seal::RelinKeys> m_relinKeysPtr{nullptr};
        std::unique_ptr<log4cplus::Logger> m_logger{nullptr};

        std::string m_publicKeyBuffer{};
    };

} // yakbas
