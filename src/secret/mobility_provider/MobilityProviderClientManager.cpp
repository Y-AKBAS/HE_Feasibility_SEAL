
#include <log4cplus/loggingmacros.h>
#include "MobilityProviderClientManager.h"
#include "ApplicationConstants.h"

namespace yakbas::sec {
    MobilityProviderClientManager::MobilityProviderClientManager(const std::string &&loggerInstance)
            : m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance(loggerInstance))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Secret Mobility Provider Client Manager is being initialized...");
            const std::map<std::string, std::string> namePortPair{
                    {constants::TRANSPORT_CHANNEL, SECRET_TRANSPORTER_SERVER_PORT}
            };
            MobilityProviderClientManager::CreateChannels(namePortPair);
        });
    }

    std::once_flag MobilityProviderClientManager::m_isInitialized{};

    void MobilityProviderClientManager::GetPublicKey() const {throw std::bad_function_call();}

} // yakbas