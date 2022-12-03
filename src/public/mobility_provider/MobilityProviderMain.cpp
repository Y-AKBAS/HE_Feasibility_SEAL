
#include "MobilityProviderApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::pub::MobilityProviderApplication>();
    application->Run();
    return 0;
}


