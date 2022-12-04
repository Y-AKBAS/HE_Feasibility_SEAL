
#include "TransporterApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::pub::TransporterApplication>();
    application->Run();
    return 0;
}


