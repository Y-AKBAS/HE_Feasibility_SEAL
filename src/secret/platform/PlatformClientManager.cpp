
#include <log4cplus/loggingmacros.h>
#include "PlatformClientManager.h"
#include "ApplicationConstants.h"

namespace yakbas::sec {
    PlatformClientManager::PlatformClientManager()
            : m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("Platform Client Manager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Client Manager is being initialized...");
            PlatformClientManager::CreateChannels({
                                                          {constants::MOBILITY_PROVIDER_CHANNEL,
                                                           SECRET_MOBILITY_PROVIDER_SERVER_PORT}
                                                  });
        });
    }

    PlatformClientManager::~PlatformClientManager() = default;

    bool PlatformClientManager::IsInitialized() {
        return !m_channelMap.empty();
    }

    void PlatformClientManager::GetPublicKey() const {}

} // yakbas