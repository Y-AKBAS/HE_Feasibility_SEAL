
#include "IApplication.h"
#include "SecretBaseApplication.h"

namespace yakbas::sec {

    class TransporterApplication : public SecretBaseApplication {
    public:
        ~TransporterApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;

    };

} // yakbas
