#pragma once

#include <string>

namespace yakbas {

    struct BaseCommandLineInfo {
        int m_numberOfRequests{5};
        int timeUnit{2};
        std::string m_portUrl{};

        BaseCommandLineInfo();

        virtual ~BaseCommandLineInfo();
    };

} // yakbas
