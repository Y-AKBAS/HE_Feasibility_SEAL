#pragma once

#include "IApplication.h"
#include "SecretCommandLineInfo.h"

namespace yakbas::sec {

    class SecretBaseApplication : public IApplication {

    public:
        [[nodiscard]] static std::unique_ptr<SecretCommandLineInfo>
        HandleCommandLine(int argc, char **argv, const std::string &&loggerInstance);
    };

} // yakbas
