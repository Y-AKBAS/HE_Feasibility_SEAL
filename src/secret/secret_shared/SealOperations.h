#pragma once

#include <string>
#include <memory>
#include "seal/seal.h"
#include "SealInfo.h"
#include <log4cplus/logger.h>

namespace yakbas::sec {

    class SealOperations {
    public:
        explicit SealOperations(const SealKeys &sealKeys = {});

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> Encrypt(const std::uint64_t &num,
                                                                const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        EncryptSymmetric(const uint64_t &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::uint64_t Decrypt(const seal::Ciphertext &cipher,
                                            seal::Decryptor &decryptor) const;

        [[nodiscard]] std::unique_ptr<std::string>
        GetEncryptedBuffer(const uint64_t &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetCipherFromBuffer(std::stringstream &stream) const;

        [[nodiscard]] std::unique_ptr<std::string>
        GetSymmetricEncryptedBuffer(const uint64_t &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::uint64_t DecryptFromBuffer(std::stringstream &stream,
                                                      seal::Decryptor &decryptor) const;

        [[nodiscard]] std::unique_ptr<seal::PublicKey>
        GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] const std::unique_ptr<SealInfo> &GetSealInfoPtr() const;

        bool operator==(const SealOperations &rhs) const;

    private:

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> EncodeAndEncrypt(const std::uint64_t &num,
                                                                         const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> EncodeAndEncryptSymmetric(const std::uint64_t &num,
                                                                                  const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::uint64_t DecodeAndDecrypt(const seal::Ciphertext &cipher,
                                                     seal::Decryptor &decryptor) const;

        const std::unique_ptr<SealInfo> m_sealInfoPtr{nullptr};
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        const seal::scheme_type m_schemeType;
        std::unique_ptr<seal::CKKSEncoder> m_ckksEncoder{nullptr};
        std::unique_ptr<seal::BatchEncoder> m_batchEncoder{nullptr};
        const static double m_scale;
    };

} // yakbas
