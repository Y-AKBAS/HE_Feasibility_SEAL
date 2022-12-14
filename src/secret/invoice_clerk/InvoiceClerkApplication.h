
#include "IApplication.h"

namespace yakbas::sec {

    class InvoiceClerkApplication : public IApplication {
    public:
        ~InvoiceClerkApplication() override;

        void Run(int argc, char **argv) override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
