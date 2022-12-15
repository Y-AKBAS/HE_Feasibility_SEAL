#pragma once

#include <string>
#include <memory>
#include "seal/seal.h"
#include "SealInfo.h"

namespace yakbas::sec {

    class SealOperations {
    public:
        explicit SealOperations(const SealKeys &mSealKeys = {seal::scheme_type::bfv, 16384, 1024});

        [[nodiscard]] std::unique_ptr<seal::Ciphertext> Encrypt(const std::uint64_t &num,
                                                                const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::uint64_t Decrypt(const seal::Ciphertext &cipher,
                                            seal::Decryptor &decryptor) const;

        [[nodiscard]] std::unique_ptr<std::string>
        GetEncryptedBuffer(const uint64_t &num, const seal::Encryptor &encryptor) const;

        [[nodiscard]] std::unique_ptr<seal::Ciphertext>
        GetCipherFromBuffer(std::stringstream &stream) const;

        [[nodiscard]] std::uint64_t DecryptFromBuffer(std::stringstream &stream,
                                                      seal::Decryptor &decryptor) const;

        std::unique_ptr<seal::PublicKey> GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const;

        [[nodiscard]] const std::unique_ptr<SealInfo> &GetSealInfoPtr() const;

        bool operator==(const SealOperations &rhs) const;

    private:
        const std::unique_ptr<SealInfo> m_sealInfoPtr;
    };

} // yakbas
