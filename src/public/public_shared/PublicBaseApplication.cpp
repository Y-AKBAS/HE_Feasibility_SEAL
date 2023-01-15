
#include "PublicBaseApplication.h"

namespace yakbas::pub {

    std::unique_ptr<PublicCommandLineInfo>
    PublicBaseApplication::HandleCommandLine(int argc, char **argv, const std::string &&loggerInstance) {
        return std::make_unique<PublicCommandLineInfo>();
    }
} // yakbas
