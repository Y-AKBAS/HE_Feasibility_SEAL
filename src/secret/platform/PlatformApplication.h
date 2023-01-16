
#include "IApplication.h"
#include "SecretBaseApplication.h"

namespace yakbas::sec {

    class PlatformApplication : public SecretBaseApplication {
    public:
        ~PlatformApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
