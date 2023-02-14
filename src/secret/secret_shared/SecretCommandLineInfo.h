#pragma once

#include "SealInfo.h"
#include "BaseCommandLineInfo.h"

namespace yakbas::sec {

    struct SecretCommandLineInfo : BaseCommandLineInfo {
        SealKeys m_sealKeys{};
        bool m_isContext{false};

        bool operator==(const SecretCommandLineInfo &rhs) const;

        SecretCommandLineInfo();

        ~SecretCommandLineInfo() override;
    };

} // yakbas
