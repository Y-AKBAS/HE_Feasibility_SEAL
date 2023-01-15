#include "BaseCommandLineInfo.h"

namespace yakbas {
    BaseCommandLineInfo::~BaseCommandLineInfo() = default;

    BaseCommandLineInfo::BaseCommandLineInfo() = default;

    BaseCommandLineInfo::BaseCommandLineInfo(int numberOfRequests)
            : m_numberOfRequests(numberOfRequests) {}
} // yakbas