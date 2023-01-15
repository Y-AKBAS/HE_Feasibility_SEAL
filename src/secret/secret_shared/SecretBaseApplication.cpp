#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "SecretBaseCommandLineParser.h"
#include "SecretBaseApplication.h"

namespace yakbas::sec {

    std::unique_ptr<SecretCommandLineInfo>
    SecretBaseApplication::HandleCommandLine(int argc, char **argv, const std::string &&loggerInstance) {

        log4cplus::Logger logger = log4cplus::Logger::getInstance(loggerInstance);

        std::vector<std::string> args(argv + 1, argv + argc);
        auto cmdLineParserResultPair = SecretBaseCommandLineParser::GetCommandLineInfo(args);

        auto resultPtr = PrintCmdLineStateAndExtractPtr<SecretCommandLineInfo>(
                logger, cmdLineParserResultPair
        );

        LOG4CPLUS_INFO(logger, resultPtr->m_sealKeys.ToString());
        return resultPtr;
    }
} // yakbas
