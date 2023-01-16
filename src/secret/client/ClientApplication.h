
#include "IApplication.h"
#include "SecretCommandLineInfo.h"
#include "SecretBaseApplication.h"
#include <boost/program_options.hpp>

namespace yakbas::sec {

    class ClientApplication : public SecretBaseApplication {
    public:
        ~ClientApplication() override;

        void Run(int argc, char **argv) override;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;

    };

} // yakbas
