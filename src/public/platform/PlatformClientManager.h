#pragma once

#include <log4cplus/logger.h>
#include "BaseClientManager.h"
#include "PublicBaseClientManager.h"

namespace yakbas::pub {

    class PlatformClientManager : public PublicBaseClientManager {

    public:
        PlatformClientManager();

        ~PlatformClientManager() override;

    private:
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        static std::once_flag m_isInitialized;
    };

} // yakbas
