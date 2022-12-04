
#include "TransporterApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::sec::TransporterApplication>();
    application->Run();
    return 0;
}


