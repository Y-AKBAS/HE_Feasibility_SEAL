#pragma once

#include "BaseClientManager.h"
#include "PublicBaseClientManager.h"

namespace yakbas::pub {

    class MobilityProviderClientManager : public PublicBaseClientManager {
    public:
        explicit MobilityProviderClientManager(const std::string &&loggerInstance);

    private:
        static std::once_flag m_isInitialized;
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
    };

} // yakbas
