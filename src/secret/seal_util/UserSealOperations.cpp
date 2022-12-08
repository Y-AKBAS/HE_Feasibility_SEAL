
#include <log4cplus/loggingmacros.h>
#include "UserSealOperations.h"
#include "Utils.h"

namespace yakbas::sec {

    using namespace yakbas::util;

    UserSealOperations::UserSealOperations(const std::string &&loggerInstance, const SealKeys &sealKeys)
            : m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance(loggerInstance))) {

        const auto &operations =
                UserSealOperations::getOperations(sealKeys);

        m_publicKeyPtr = getUnique<seal::PublicKey>();
        operations.GetSealInfoPtr()->m_keyGeneratorPtr->create_public_key(*m_publicKeyPtr);

        m_secretKeyPtr = getUnique<seal::SecretKey>(
                operations.GetSealInfoPtr()->m_keyGeneratorPtr->secret_key());

        m_encryptorPtr = getUnique<seal::Encryptor>(
                *operations.GetSealInfoPtr()->m_sealContextPtr,
                *m_publicKeyPtr);

        m_decryptorPtr = getUnique<seal::Decryptor>(
                *operations.GetSealInfoPtr()->m_sealContextPtr,
                *m_secretKeyPtr);
    }

    // Doesn't cause memory leak :)
    const SealOperations &
    UserSealOperations::getOperations(const SealKeys &sealKeys) {
        static const auto logger = getUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("MethodLogger"));
        static std::vector<const SealOperations *> operations{};

        const auto it = std::ranges::find_if(operations, [&sealKeys](const SealOperations *operationsPtr) {
            return operationsPtr->GetSealInfoPtr()->m_sealKeys == sealKeys;
        });

        if (it != std::end(operations)) {
            LOG4CPLUS_DEBUG(*logger, "Found existing SealKeys. They will be returned..." );
            return *(*it);
        } else {
            LOG4CPLUS_DEBUG(*logger, "Creating new SealKeys with params: \n" + sealKeys.ToString());
            const auto newOperationsPtr = new SealOperations(sealKeys);
            operations.push_back(newOperationsPtr);
            return *newOperationsPtr;
        }
    }

} // yakbas