
#include "IApplication.h"

namespace yakbas::sec {

    class MobilityProviderApplication : public IApplication {
    public:
        ~MobilityProviderApplication() override;

        void Run() override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
