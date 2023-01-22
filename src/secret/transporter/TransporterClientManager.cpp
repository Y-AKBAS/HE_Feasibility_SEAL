
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "TransporterClientManager.h"
#include "ApplicationConstants.h"

namespace yakbas::sec {
    TransporterClientManager::TransporterClientManager(const SealKeys &sealKeys)
            : m_schemeType(sealKeys.m_schemeType),
              m_logger(std::make_unique<log4cplus::Logger>(log4cplus::Logger::getInstance("SecretClientManager"))) {

        std::call_once(m_isInitialized, [this]() {
            LOG4CPLUS_DEBUG(*m_logger, "Secret Transporter Client Manager is being initialized...");
            TransporterClientManager::CreateChannels(
                    {{constants::PLATFORM_CHANNEL, SECRET_PLATFORM_SERVER_PORT}}
            );
        });
    }

    TransporterClientManager::~TransporterClientManager() = default;

    std::once_flag TransporterClientManager::m_isInitialized{};

    void TransporterClientManager::GetPublicKey() const {
        throw std::bad_function_call();
    }

} // sec