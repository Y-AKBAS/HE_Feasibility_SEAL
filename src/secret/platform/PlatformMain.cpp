
#include "PlatformApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::sec::PlatformApplication>();
    application->Run();
    return 0;
}


