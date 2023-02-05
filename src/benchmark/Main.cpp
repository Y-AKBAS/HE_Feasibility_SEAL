#include "Application.h"

int main(int argc, char **argv) {
    const auto application = std::make_unique<yakbas::Application>();
    application->Run(argc, argv);
    return 0;
}
