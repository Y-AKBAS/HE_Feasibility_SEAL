
#include "SecretCommandLineInfo.h"

namespace yakbas::sec {

    bool SecretCommandLineInfo::operator==(const SecretCommandLineInfo &secretCommandLineInfo) const {
        return this->m_sealKeys == secretCommandLineInfo.m_sealKeys &&
               this->m_numberOfRequests == secretCommandLineInfo.m_numberOfRequests;
    }

    SecretCommandLineInfo::~SecretCommandLineInfo() = default;

    SecretCommandLineInfo::SecretCommandLineInfo() = default;

} // yakbas
