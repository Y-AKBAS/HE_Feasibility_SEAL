#pragma once

#include "BaseCommandLineInfo.h"

namespace yakbas::pub {

    struct PublicCommandLineInfo : public BaseCommandLineInfo {
        ~PublicCommandLineInfo() override;

        bool operator==(const PublicCommandLineInfo &rhs) const;
    };

} // yakbas
