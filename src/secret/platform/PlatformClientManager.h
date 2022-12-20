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
        std::once_flag m_isInitialized;
        std::unique_ptr<log4cplus::Logger> m_logger;
    };

} // yakbas
