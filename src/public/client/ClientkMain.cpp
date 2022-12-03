
#include "ClientApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::pub::ClientApplication>();
    application->Run();
    return 0;
}


