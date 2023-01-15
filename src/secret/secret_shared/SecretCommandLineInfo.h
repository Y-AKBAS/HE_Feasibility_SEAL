#pragma once

#include "SealInfo.h"
#include "BaseCommandLineInfo.h"

namespace yakbas::sec {

    struct SecretCommandLineInfo : BaseCommandLineInfo {
        SealKeys m_sealKeys{};

        bool operator==(const SecretCommandLineInfo &secretCommandLineInfo) const;

        SecretCommandLineInfo(int numberOfRequests, const SealKeys &sealKeys);

        SecretCommandLineInfo();

        ~SecretCommandLineInfo() override;
    };

} // yakbas
