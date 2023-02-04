
#include <log4cplus/loggingmacros.h>
#include "PlatformClientManager.h"
#include "ApplicationConstants.h"

namespace yakbas::sec {
    PlatformClientManager::PlatformClientManager()
            : m_logger(
            std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("Secret Platform Client Manager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Secret Platform Client Manager is being initialized...");
            const std::map<std::string, std::string> namePortPair{
                    {constants::MOBILITY_PROVIDER_CHANNEL_1, SECRET_MOBILITY_PROVIDER_SERVER_PORT_1},
                    {constants::MOBILITY_PROVIDER_CHANNEL_2, SECRET_MOBILITY_PROVIDER_SERVER_PORT_2},
                    {constants::MOBILITY_PROVIDER_CHANNEL_3, SECRET_MOBILITY_PROVIDER_SERVER_PORT_3}
            };
            PlatformClientManager::CreateChannels(namePortPair);
        });
    }

    PlatformClientManager::~PlatformClientManager() = default;

    std::once_flag PlatformClientManager::m_isInitialized{};

    bool PlatformClientManager::IsInitialized() {
        return !m_channelMap.empty();
    }

    void PlatformClientManager::GetPublicKey() const {throw std::bad_function_call();}

} // yakbas