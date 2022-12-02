
#include <iostream>
#include "Application.h"

namespace yakbas {
    void Application::Run() {
        std::cout << SECRET_INVOICE_CLERK_SERVER_PORT << std::endl;
        std::cout << typeid(SECRET_INVOICE_CLERK_SERVER_PORT).name() << std::endl;
    }

} // yakbas