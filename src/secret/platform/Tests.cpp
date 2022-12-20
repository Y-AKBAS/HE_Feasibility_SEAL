
#if (!DISABLE_TESTS)

#include "doctest/doctest.h"
#include "CustomSealOperations.h"
#include "Utils.h"
#include "PlatformClientManager.h"

#include <memory>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yakbas::sec::test {

    TEST_SUITE("Secret Platform Test Suite") {

        TEST_CASE("Secret Platform Tests") {
            const static auto logger = util::GetUnique<log4cplus::Logger>(log4cplus::Logger::getInstance("TestLogger"));

            SUBCASE("Secret Platform Manager Initialization Test") {
                const auto clientManagerPtr = std::make_unique<PlatformClientManager>();
                CHECK(PlatformClientManager::IsInitialized());
            }
        }
    }
}

#endif