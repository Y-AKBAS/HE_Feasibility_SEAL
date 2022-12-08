#pragma once

#include <log4cplus/logger.h>
#include "SealOperations.h"

namespace yakbas::sec {

    class UserSealOperations {
    public:

        explicit UserSealOperations(const std::string &&loggerInstance,
                                    const SealKeys &sealKeys = {seal::scheme_type::bfv, 16384, 1024});

        static const SealOperations &getOperations(const SealKeys &sealKeys = {seal::scheme_type::bfv, 16384, 1024});

    private:
        std::unique_ptr<seal::PublicKey> m_publicKeyPtr{nullptr};
        std::unique_ptr<seal::SecretKey> m_secretKeyPtr{nullptr};
        std::unique_ptr<seal::Encryptor> m_encryptorPtr{nullptr};
        std::unique_ptr<seal::Decryptor> m_decryptorPtr{nullptr};
        std::unique_ptr<log4cplus::Logger> m_logger;
    };

} // yakbas
