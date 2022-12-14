
#include "IApplication.h"

namespace yakbas::pub {

    class ClientApplication : public IApplication {
    public:
        ~ClientApplication() override;

        void Run(int argc, char **argv) override;

        void EnableLogging() override;

        void StartServer() override;

    };

} // yakbas
