
#include "IApplication.h"
#include "SecretBaseApplication.h"

namespace yakbas::sec {

    class MobilityProviderApplication : public SecretBaseApplication {
    public:
        ~MobilityProviderApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
