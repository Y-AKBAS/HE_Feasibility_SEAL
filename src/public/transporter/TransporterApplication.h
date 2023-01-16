
#include "IApplication.h"
#include "PublicBaseApplication.h"

namespace yakbas::pub {

    class TransporterApplication : public PublicBaseApplication {
    public:
        ~TransporterApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
