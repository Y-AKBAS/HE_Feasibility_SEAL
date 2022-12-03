
#include "InvoiceClerkApplication.h"
#include <memory>

int main() {
    std::make_unique<yakbas::pub::InvoiceClerkApplication>()->Run();
    return 0;
}


