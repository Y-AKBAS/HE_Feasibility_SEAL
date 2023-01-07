#pragma once

#if (DISABLE_TESTS)
#define DOCTEST_CONFIG_DISABLE
#else
#define DOCTEST_CONFIG_IMPLEMENT
#endif

namespace yakbas {

    class IApplication {
    public:
        virtual ~IApplication() = default;

        virtual void StartServer() = 0;

        virtual void Run(int argc, char **argv) = 0;

        virtual void EnableLogging();

        virtual void DisableLogging();

        virtual int RunTests(int argc, char **argv);
    };

} // yakbas
