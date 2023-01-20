
#include <log4cplus/loggingmacros.h>
#include "PlatformClientManager.h"
#include "ApplicationConstants.h"

namespace yakbas::pub {
    PlatformClientManager::PlatformClientManager()
            : m_logger(
            std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("Public Platform Client Manager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Public Platform Client Manager is being initialized...");
            const std::map<std::string, std::string> namePortPair{
                    {constants::MOBILITY_PROVIDER_CHANNEL_1, PUBLIC_MOBILITY_PROVIDER_SERVER_PORT_1},
                    {constants::MOBILITY_PROVIDER_CHANNEL_2, PUBLIC_MOBILITY_PROVIDER_SERVER_PORT_2},
                    {constants::MOBILITY_PROVIDER_CHANNEL_3, PUBLIC_MOBILITY_PROVIDER_SERVER_PORT_3}
            };
            PlatformClientManager::CreateChannels(namePortPair);
        });
    }

    PlatformClientManager::~PlatformClientManager() = default;

    std::once_flag PlatformClientManager::m_isInitialized{};

} // yakbas