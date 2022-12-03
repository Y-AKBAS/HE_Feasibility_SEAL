
#include "IApplication.h"

namespace yakbas::pub {

    class MobilityProviderApplication : public IApplication {
    public:
        ~MobilityProviderApplication() override;

        void Run() override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
