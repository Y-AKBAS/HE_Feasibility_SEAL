#pragma once

#include <log4cplus/logger.h>
#include "SecretBaseClientManager.h"

namespace yakbas::sec {

    class MobilityProviderClientManager : public SecretBaseClientManager {
    public:
        explicit MobilityProviderClientManager(const std::string &&loggerInstance);

        void GetPublicKey() const override;

    private:
        static std::once_flag m_isInitialized;
        const std::unique_ptr<const log4cplus::Logger> m_logger;
    };

} // yakbas
