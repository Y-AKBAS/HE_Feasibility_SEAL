#pragma once

namespace yakbas {

    class IApplication {
    public:
        virtual ~IApplication() = default;
        virtual void EnableLogging() = 0;
        virtual void StartServer() = 0;
        virtual void Run() = 0;
    };

} // yakbas
