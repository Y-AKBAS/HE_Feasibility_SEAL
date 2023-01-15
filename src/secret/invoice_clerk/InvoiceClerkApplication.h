
#include "IApplication.h"
#include "SecretBaseApplication.h"

namespace yakbas::sec {

    class InvoiceClerkApplication : public SecretBaseApplication {
    public:
        ~InvoiceClerkApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;

    };

} // yakbas
