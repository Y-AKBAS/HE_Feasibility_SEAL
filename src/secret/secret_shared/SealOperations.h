#pragma once

#include <string>
#include <memory>
#include "seal/seal.h"
#include "SealInfo.h"
#include "Utils.h"
#include <log4cplus/logger.h>

namespace yakbas::sec {
    using namespace yakbas::util;

    class SealOperations {
    public:
        explicit SealOperations(const SealKeys &sealKeys = {});

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> Encrypt(const num_variant &num,
                                                                const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        EncryptSymmetric(const num_variant &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] num_variant Decrypt(const seal::Ciphertext &cipher,
                                          seal::Decryptor &decryptor) const;

        void AddProcessedInPlace(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd,
                                 const seal::Evaluator &evaluator) const;

        void SubProcessedInPlace(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd,
                                 const seal::Evaluator &evaluator) const;

        void AddProcessedCiphers(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd,
                                 seal::Ciphertext &destination,
                                 const seal::Evaluator &evaluator) const;

        void SubProcessedCiphers(seal::Ciphertext &processedCipher, seal::Ciphertext &cipherToAdd,
                                 seal::Ciphertext &destination,
                                 const seal::Evaluator &evaluator) const;

        [[nodiscard]] std::unique_ptr<std::string>
        GetEncryptedBuffer(const num_variant &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetCipherFromBuffer(std::stringstream &stream) const;

        [[nodiscard]] std::unique_ptr<std::string>
        GetSymmetricEncryptedBuffer(const num_variant &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] num_variant DecryptFromBuffer(std::stringstream &stream,
                                                    seal::Decryptor &decryptor) const;

        [[nodiscard]] std::unique_ptr<seal::PublicKey>
        GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] std::unique_ptr<seal::RelinKeys>
        GetRelinKeysFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] const std::unique_ptr<SealInfo> &GetSealInfoPtr() const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetNewCipher(const std::optional<seal::parms_id_type> &parms_id) const;

        static void
        Relinearize(seal::Ciphertext &ciphertext, const seal::Evaluator &evaluator, const seal::RelinKeys &relinKeys);

        bool operator==(const SealOperations &rhs) const;

    private:

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> EncodeAndEncrypt(const num_variant &num,
                                                                         const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> EncodeAndEncryptSymmetric(const num_variant &num,
                                                                                  const seal::Encryptor &encryptor) const;

        [[nodiscard]] num_variant DecodeAndDecrypt(const seal::Ciphertext &cipher,
                                                   seal::Decryptor &decryptor) const;

        const std::unique_ptr<SealInfo> m_sealInfoPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const seal::scheme_type m_schemeType;
        std::unique_ptr<seal::CKKSEncoder> m_ckksEncoder{nullptr};
        std::unique_ptr<seal::BatchEncoder> m_batchEncoder{nullptr};
        const double m_scale{};
        const int m_scalePower{};
    };

} // yakbas
