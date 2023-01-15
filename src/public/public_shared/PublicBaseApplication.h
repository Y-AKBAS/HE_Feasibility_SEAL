#pragma once

#include "IApplication.h"
#include "PublicCommandLineInfo.h"
#include <memory>
#include <string>

namespace yakbas::pub {

    class PublicBaseApplication : public IApplication {

    public:
        [[nodiscard]] static std::unique_ptr<PublicCommandLineInfo>
        HandleCommandLine(int argc, char **argv, const std::string &&loggerInstance);
    };

} // yakbas
