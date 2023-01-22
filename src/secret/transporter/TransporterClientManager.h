#pragma once

#include <seal/encryptionparams.h>
#include "BaseClientManager.h"
#include "SealInfo.h"
#include "SecretBaseClientManager.h"

namespace yakbas::sec {

    class TransporterClientManager : public SecretBaseClientManager {
    public:
        explicit TransporterClientManager(const SealKeys &sealKeys = {});

        void GetPublicKey() const override;

        ~TransporterClientManager() override;

    private:
        const seal::scheme_type m_schemeType;
        const std::unique_ptr<log4cplus::Logger> m_logger{nullptr};
        static std::once_flag m_isInitialized;
    };

} // yakbas
