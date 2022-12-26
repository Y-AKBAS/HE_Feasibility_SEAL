#pragma once

#include <log4cplus/logger.h>
#include "BaseClientManager.h"
#include "SecretBaseClientManager.h"

namespace yakbas::sec {

    class PlatformClientManager : public SecretBaseClientManager {
    public:
        PlatformClientManager();

        ~PlatformClientManager() override;

        static bool IsInitialized();

    protected:
        void GetPublicKey() const override;

    private:
        static std::once_flag m_isInitialized;
        const std::unique_ptr<const log4cplus::Logger> m_logger;
    };

} // yakbas
