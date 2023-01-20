#pragma once

#include "BaseCommandLineInfo.h"
#include "BaseServerManager.h"
#include <memory>
#include <vector>

#if (DISABLE_TESTS)
#define DOCTEST_CONFIG_DISABLE
#else
#define DOCTEST_CONFIG_IMPLEMENT
#endif

namespace yakbas {

    class IApplication {
    public:
        virtual ~IApplication() = default;

        virtual void StartServer(BaseCommandLineInfo *commandLineInfoPtr) = 0;

        virtual void Run(int argc, char **argv) = 0;

        virtual void EnableLogging();

        virtual void DisableLogging();

        virtual int RunTests(int argc, char **argv);

    protected:
        std::vector<std::unique_ptr<BaseServerManager>> m_serverManagersPtr{};
    };

} // yakbas
