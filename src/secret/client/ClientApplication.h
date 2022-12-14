
#include "IApplication.h"

namespace yakbas::sec {

    class ClientApplication : public IApplication {
    public:
        ~ClientApplication() override;

        void Run(int argc, char **argv) override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
