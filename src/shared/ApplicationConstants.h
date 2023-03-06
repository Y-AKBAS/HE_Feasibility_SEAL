#pragma once

#include <string>
#include <utility>
#include <vector>

namespace yakbas::constants {
    extern const std::string INVOICE_CLERK_CHANNEL;
    extern const std::string MOBILITY_PROVIDER_CHANNEL_1;
    extern const std::string MOBILITY_PROVIDER_CHANNEL_2;
    extern const std::string MOBILITY_PROVIDER_CHANNEL_3;
    extern const std::string PLATFORM_CHANNEL;
    extern const std::string CLIENT_CHANNEL;
    extern const std::string TRANSPORT_CHANNEL;
    extern const constexpr int APP_MAX_RANDOM_NUMBER;
    extern const constexpr int APP_MIN_RANDOM_NUMBER;
    extern const constexpr int NUMBER_OF_RIDES_PER_JOURNEY;
    extern const std::pair<const char *, bool> DOCKTEST_NO_RUN;
    extern const std::pair<const char *, bool> DOCKTEST_NO_BREAKS;
    extern const std::pair<const char *, int> DOCKTEST_ABORT_AFTER;
    extern const std::vector<int> ENCODING_BIT_SIZES;

} // yakbas
