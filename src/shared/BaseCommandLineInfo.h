#pragma once

#include <string>

namespace yakbas {

    struct BaseCommandLineInfo {
        int m_numberOfRequests{5};
        int timeUnit{2};
        bool m_isSecret{true};
        std::string m_portUrl{};

        BaseCommandLineInfo();

        virtual ~BaseCommandLineInfo();
    };

} // yakbas
