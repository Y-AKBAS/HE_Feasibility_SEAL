#pragma once

#ifdef DISABLE_TESTS
#define DOCTEST_CONFIG_DISABLE
#else
#define DOCTEST_CONFIG_IMPLEMENT
#endif

namespace yakbas {

    class IApplication {
    public:
        virtual ~IApplication() = default;

        virtual void EnableLogging() = 0;

        virtual void StartServer() = 0;

        virtual void Run() = 0;

        // default implementation
        int RunTests(int argc, char **argv);
    };

} // yakbas
