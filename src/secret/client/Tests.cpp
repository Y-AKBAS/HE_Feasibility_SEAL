
#if (!DISABLE_TESTS)

#include "doctest/doctest.h"
#include "CustomSealOperations.h"
#include "Utils.h"
#include "ClientManager.h"
#include <memory>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec::test {

    TEST_SUITE("Secret Client Test Suite") {

        TEST_CASE("Secret Client Tests") {
            ::log4cplus::initialize();
            ::log4cplus::PropertyConfigurator::doConfigure(DEFAULT_LOG_CONFIG_FILE_NAME);
            const auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            SUBCASE("Client Manager Initialization Test") {
                LOG4CPLUS_DEBUG(*logger, "Test is running...");
                const auto clientManagerPtr = std::make_unique<ClientManager>();
                CHECK(ClientManager::IsInitialized() == true);
            }

            ::log4cplus::deinitialize();
        }
    }

}

#endif