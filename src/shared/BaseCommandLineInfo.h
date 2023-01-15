#pragma once

namespace yakbas {

    struct BaseCommandLineInfo {
        int m_numberOfRequests{5};

        explicit BaseCommandLineInfo(int numberOfRequests);

        BaseCommandLineInfo();

        virtual ~BaseCommandLineInfo();
    };

} // yakbas
