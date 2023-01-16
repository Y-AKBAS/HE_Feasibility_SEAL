
#include "IApplication.h"
#include "PublicBaseApplication.h"

namespace yakbas::pub {

    class InvoiceClerkApplication : public PublicBaseApplication {
    public:
        ~InvoiceClerkApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
