
#include "IApplication.h"

namespace yakbas::sec {

    class PlatformApplication : public IApplication {
    public:
        ~PlatformApplication() override;

        void Run() override;

        void EnableLogging() override;

        void StartServer() override;
    };

} // yakbas
