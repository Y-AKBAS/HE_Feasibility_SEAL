
#include <log4cplus/loggingmacros.h>
#include "CustomSealOperations.h"
#include "Utils.h"

namespace yakbas::sec {

    using namespace yakbas::util;

    CustomSealOperations::CustomSealOperations(const SealKeys &sealKeys)
            : m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("CustomSealOperations"))),
              m_sealOperations(&CustomSealOperations::GetOperations(sealKeys)) {

        m_publicKeyPtr = GetUnique<seal::PublicKey>();
        m_sealOperations->GetSealInfoPtr()->m_keyGeneratorPtr->create_public_key(*m_publicKeyPtr);

        m_secretKeyPtr = GetUnique<seal::SecretKey>(
                m_sealOperations->GetSealInfoPtr()->m_keyGeneratorPtr->secret_key());

        m_encryptorPtr = GetUnique<seal::Encryptor>(
                *m_sealOperations->GetSealInfoPtr()->m_sealContextPtr,
                *m_publicKeyPtr);

        m_decryptorPtr = GetUnique<seal::Decryptor>(
                *m_sealOperations->GetSealInfoPtr()->m_sealContextPtr,
                *m_secretKeyPtr);
    }

    // No worries. Doesn't cause memory leak :)
    const SealOperations &
    CustomSealOperations::GetOperations(const SealKeys &sealKeys) {
        static const auto logger
                = GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("GetOperations_Logger"));
        static std::vector<const SealOperations *> operations{};

        const auto it = std::ranges::find_if(operations, [&sealKeys](const SealOperations *operationsPtr) {
            return operationsPtr->GetSealInfoPtr()->m_sealKeys == sealKeys;
        });

        if (it != std::end(operations)) {
            LOG4CPLUS_DEBUG(*logger, "Found existing SealKeys. They will be returned...");
            return *(*it);
        } else {
            LOG4CPLUS_DEBUG(*logger, "Creating new with params: \n" + sealKeys.ToString());
            const auto newOperationsPtr = new SealOperations(sealKeys);
            operations.push_back(newOperationsPtr);
            return *newOperationsPtr;
        }
    }

    std::unique_ptr<seal::Ciphertext> CustomSealOperations::Encrypt(const uint64_t &num) const {
        return m_sealOperations->Encrypt(num, *m_encryptorPtr);
    }

    std::uint64_t CustomSealOperations::Decrypt(const seal::Ciphertext &cipher) const {
        return m_sealOperations->Decrypt(cipher, *m_decryptorPtr);
    }

    std::unique_ptr<std::string> CustomSealOperations::GetEncryptedBuffer(const uint64_t &num) const {
        return m_sealOperations->GetEncryptedBuffer(num, *m_encryptorPtr);
    }

    std::unique_ptr<seal::Ciphertext>
    CustomSealOperations::GetCipherFromBuffer(const std::unique_ptr<std::stringstream> &stream) const {
        return m_sealOperations->GetCipherFromBuffer(*stream);
    }

    std::uint64_t CustomSealOperations::DecryptFromBuffer(const std::unique_ptr<std::stringstream> &stream) const {
        return m_sealOperations->DecryptFromBuffer(*stream, *m_decryptorPtr);
    }

    const SealOperations *CustomSealOperations::GetSealOperations() const {
        return m_sealOperations;
    }

    std::unique_ptr<seal::PublicKey>
    CustomSealOperations::GetPublicKeyFromBuffer(const std::unique_ptr<std::stringstream> &stream) const{
        return m_sealOperations->GetPublicKeyFromBuffer(stream);
    }

    std::unique_ptr<std::stringstream> CustomSealOperations::PublicKeyToBuffer() const {
        auto stream = GetUniqueStream();
        m_publicKeyPtr->save(*stream);
        return stream;
    }


} // yakbas