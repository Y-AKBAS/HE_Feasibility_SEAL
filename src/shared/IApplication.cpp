
#include "IApplication.h"
#include <memory>
#include "doctest/doctest.h"

namespace yakbas {

    int yakbas::IApplication::RunTests(int argc, char **argv) {
        const auto context = std::make_unique<doctest::Context>();
        context->setOption("abort-after", 1);
        context->setOption("no-breaks", true);
        context->applyCommandLine(argc, argv);

        int result = context->run();
        if (context->shouldExit()) {
            return result;
        }
        return -1;
    }
}