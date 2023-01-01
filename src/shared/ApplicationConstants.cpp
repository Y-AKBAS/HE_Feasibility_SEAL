
#include "ApplicationConstants.h"

namespace yakbas::constants {
    const std::string INVOICE_CLERK_CHANNEL = "invoiceClerkChannel";
    const std::string MOBILITY_PROVIDER_CHANNEL = "mobilityProviderChannel";
    const std::string PLATFORM_CHANNEL = "platformChannel";
    const std::string CLIENT_CHANNEL = "clientChannel";
    const std::string TRANSPORT_CHANNEL = "transportChannel";
    const std::pair<const char *, bool> DOCKTEST_NO_RUN = std::make_pair("no-run", false);
    const std::pair<const char *, bool> DOCKTEST_NO_BREAKS = std::make_pair("no-breaks", true);
    const std::pair<const char *, int> DOCKTEST_ABORT_AFTER = std::make_pair("abort-after", 1);
    const int APP_MAX_RANDOM_NUMBER = 18;
    const int APP_MIN_RANDOM_NUMBER = 2;

} // yakbas
