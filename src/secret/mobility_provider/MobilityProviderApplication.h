
#include "IApplication.h"

namespace yakbas::sec {

    class MobilityProviderApplication : public IApplication {
    public:
        ~MobilityProviderApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer() override;


    };

} // yakbas
