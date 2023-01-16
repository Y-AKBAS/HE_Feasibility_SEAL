
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "PublicBaseApplication.h"
#include "PublicBaseCommandLineParser.h"

namespace yakbas::pub {

    std::unique_ptr<PublicCommandLineInfo>
    PublicBaseApplication::HandleCommandLine(int argc, char **argv, const std::string &&loggerInstance) {

        log4cplus::Logger logger = log4cplus::Logger::getInstance(loggerInstance);

        std::vector<std::string> args(argv + 1, argv + argc);
        auto cmdLineParserResultPair = PublicBaseCommandLineParser::GetCommandLineInfo(args);

        return PrintCmdLineStateAndExtractPtr<PublicCommandLineInfo>(
                logger, cmdLineParserResultPair
        );
    }
} // yakbas
