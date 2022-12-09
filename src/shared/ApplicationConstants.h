#pragma once

#include <string>
#include <utility>

namespace yakbas::constants {
    extern const std::string INVOICE_CLERK_CHANNEL;
    extern const std::string MOBILITY_PROVIDER_CHANNEL;
    extern const std::string PLATFORM_CHANNEL;
    extern const std::string CLIENT_CHANNEL;
    extern const std::string TRANSPORT_CHANNEL;
    extern const std::pair<const char *, bool> DOCKTEST_NO_RUN;
    extern const std::pair<const char *, bool> DOCKTEST_NO_BREAKS;
    extern const std::pair<const char *, int> DOCKTEST_ABORT_AFTER;
} // yakbas
