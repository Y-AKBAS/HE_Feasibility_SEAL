
#include "IApplication.h"

namespace yakbas::sec {

    class TransporterApplication : public IApplication {
    public:
        ~TransporterApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer() override;


    };

} // yakbas
