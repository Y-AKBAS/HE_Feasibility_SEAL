
#include "IApplication.h"

namespace yakbas::pub {

    class PlatformApplication : public IApplication {
    public:
        ~PlatformApplication() override;

        void Run() override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
