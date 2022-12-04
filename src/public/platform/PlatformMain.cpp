
#include "PlatformApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::pub::PlatformApplication>();
    application->Run();
    return 0;
}


