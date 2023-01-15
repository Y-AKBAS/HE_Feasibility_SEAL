
#include "IApplication.h"
#include "PublicBaseApplication.h"

namespace yakbas::pub {

    class ClientApplication : public PublicBaseApplication {
    public:
        ~ClientApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;
    };

} // yakbas
