
#include "ApplicationConstants.h"

namespace yakbas::constants {
    const std::string INVOICE_CLERK_CHANNEL = "invoiceClerkChannel";
    const std::string MOBILITY_PROVIDER_CHANNEL_1 = "mobilityProviderChannel_1";
    const std::string MOBILITY_PROVIDER_CHANNEL_2 = "mobilityProviderChannel_2";
    const std::string MOBILITY_PROVIDER_CHANNEL_3 = "mobilityProviderChannel_3";
    const std::string PLATFORM_CHANNEL = "platformChannel";
    const std::string CLIENT_CHANNEL = "clientChannel";
    const std::string TRANSPORT_CHANNEL = "transportChannel";
    const int APP_MAX_RANDOM_NUMBER = 18;
    const int APP_MIN_RANDOM_NUMBER = 2;
    const std::pair<const char *, bool> DOCKTEST_NO_RUN = std::make_pair("no-run", false);
    const std::pair<const char *, bool> DOCKTEST_NO_BREAKS = std::make_pair("no-breaks", true);
    const std::pair<const char *, int> DOCKTEST_ABORT_AFTER = std::make_pair("abort-after", 10000);
    const std::vector<int> ENCODING_BIT_SIZES{60, 40, 40, 40, 60};

} // yakbas
