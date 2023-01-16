
#include "IApplication.h"
#include "PublicBaseApplication.h"

namespace yakbas::pub {

    class MobilityProviderApplication : public PublicBaseApplication {
    public:
        ~MobilityProviderApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
