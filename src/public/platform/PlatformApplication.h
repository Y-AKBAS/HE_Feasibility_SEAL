
#include "IApplication.h"
#include "PublicBaseApplication.h"

namespace yakbas::pub {

    class PlatformApplication : public PublicBaseApplication {
    public:
        ~PlatformApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
