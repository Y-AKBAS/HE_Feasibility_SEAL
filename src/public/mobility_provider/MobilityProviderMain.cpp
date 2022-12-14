
#include "MobilityProviderApplication.h"
#include <memory>

int main(int argc, char **argv) {
    const auto application = std::make_unique<yakbas::pub::MobilityProviderApplication>();
    application->Run(argc, argv);
    return 0;
}


