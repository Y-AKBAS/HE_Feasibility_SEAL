#pragma once

#include "IApplication.h"
#include "secret/secret_shared/SecretCommandLineInfo.h"

namespace yakbas {

    class Application : public IApplication {

    public:
        ~Application() override = default;

        void StartServer(BaseCommandLineInfo *commandLineInfoPtr) override;

        void Run(int argc, char **argv) override;

        void Benchmark(const sec::SecretCommandLineInfo &commandLineInfo, int argc, char **argv);

    };

} // yakbas
