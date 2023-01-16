
#include "SecretCommandLineInfo.h"

namespace yakbas::sec {

    bool SecretCommandLineInfo::operator==(const SecretCommandLineInfo &rhs) const {
        return this->m_sealKeys == rhs.m_sealKeys &&
               this->m_numberOfRequests == rhs.m_numberOfRequests &&
               this->m_portUrl == rhs.m_portUrl;
    }

    SecretCommandLineInfo::~SecretCommandLineInfo() = default;

    SecretCommandLineInfo::SecretCommandLineInfo() = default;

} // yakbas
