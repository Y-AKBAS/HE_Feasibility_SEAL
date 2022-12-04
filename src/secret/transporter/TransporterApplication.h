
#include "IApplication.h"

namespace yakbas::sec {

    class TransporterApplication : public IApplication {
    public:
        ~TransporterApplication() override;

        void Run() override;

        void EnableLogging() override;

        void StartServer() override;


    };

} // yakbas
