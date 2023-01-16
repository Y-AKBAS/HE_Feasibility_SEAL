
#include "PublicCommandLineInfo.h"

namespace yakbas::pub {
    PublicCommandLineInfo::~PublicCommandLineInfo() = default;

    bool PublicCommandLineInfo::operator==(const PublicCommandLineInfo &rhs) const {
        return this->m_numberOfRequests == rhs.m_numberOfRequests &&
               this->m_portUrl == rhs.m_portUrl;
    }
} // yakbas
