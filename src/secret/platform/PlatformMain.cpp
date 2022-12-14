
#include "PlatformApplication.h"
#include <memory>

int main(int argc, char **argv) {
    const auto application = std::make_unique<yakbas::sec::PlatformApplication>();
    application->Run(argc, argv);
    return 0;
}


