
#include "InvoiceClerkApplication.h"
#include <memory>

int main() {
    const auto application = std::make_unique<yakbas::sec::InvoiceClerkApplication>();
    application->Run();
    return 0;
}


