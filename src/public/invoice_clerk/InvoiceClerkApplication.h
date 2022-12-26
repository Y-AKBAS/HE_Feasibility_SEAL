
#include "IApplication.h"

namespace yakbas::pub {

    class InvoiceClerkApplication : public IApplication {
    public:
        ~InvoiceClerkApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer() override;

    };

} // yakbas
