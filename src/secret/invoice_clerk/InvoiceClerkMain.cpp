
#include "InvoiceClerkApplication.h"
#include <memory>

int main(int argc, char **argv) {
    const auto application = std::make_unique<yakbas::sec::InvoiceClerkApplication>();
    application->Run(argc, argv);
    return 0;
}


