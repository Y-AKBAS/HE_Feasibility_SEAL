
#include "IApplication.h"

namespace yakbas::pub {

    class MobilityProviderApplication : public IApplication {
    public:
        ~MobilityProviderApplication() override;

        void Run(int argc, char **argv) override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
