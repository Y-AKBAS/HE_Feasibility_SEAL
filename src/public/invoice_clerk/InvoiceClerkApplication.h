
#include "IApplication.h"

namespace yakbas::pub {

    class InvoiceClerkApplication : public IApplication {
    public:
        ~InvoiceClerkApplication() override;

        void Run() override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
