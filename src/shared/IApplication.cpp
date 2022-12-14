
#include "IApplication.h"
#include <memory>
#include "doctest/doctest.h"
#include "ApplicationConstants.h"

namespace yakbas {

    int yakbas::IApplication::RunTests(int argc, char **argv) {
#if (!DISABLE_TESTS)
        const auto context = std::make_unique<doctest::Context>();
        context->setOption(constants::DOCKTEST_ABORT_AFTER.first,
                           constants::DOCKTEST_ABORT_AFTER.second);
        context->setOption(constants::DOCKTEST_NO_RUN.first,
                           constants::DOCKTEST_NO_RUN.second); // Will prevent running tests
        context->setOption(constants::DOCKTEST_NO_BREAKS.first,
                           constants::DOCKTEST_NO_BREAKS.second);
        context->applyCommandLine(argc, argv);

        int result = context->run();
        if (context->shouldExit()) {
            return result;
        }
#endif
        return -1;
    }
}
