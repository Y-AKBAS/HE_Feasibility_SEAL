
#include "ClientApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::sec::ClientApplication>();
    application->Run();
    return 0;
}


