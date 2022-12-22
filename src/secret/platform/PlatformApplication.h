
#include "IApplication.h"

namespace yakbas::sec {

    class PlatformApplication : public IApplication {
    public:
        ~PlatformApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer() override;
    };

} // yakbas
