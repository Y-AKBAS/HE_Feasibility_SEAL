
#include "ClientApplication.h"
#include <memory>

int main(int argc, char **argv) {
    const auto application = std::make_unique<yakbas::sec::ClientApplication>();
    application->Run(argc, argv);
    return 0;
}
