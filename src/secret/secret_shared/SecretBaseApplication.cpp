#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "SecretBaseCommandLineParser.h"
#include "SecretBaseApplication.h"

namespace yakbas::sec {

    std::unique_ptr<SecretCommandLineInfo>
    SecretBaseApplication::HandleCommandLine(int argc, char **argv, const std::string &&loggerInstance) {

        log4cplus::Logger logger = log4cplus::Logger::getInstance(loggerInstance);
        std::unique_ptr<SecretCommandLineInfo> commandLineInfoPtr{nullptr};

        std::vector<std::string> args(argv + 1, argv + argc);
        auto cmdLineParserResultPair = SecretBaseCommandLineParser::GetCommandLineInfo(args);

        if (cmdLineParserResultPair.second == ParserResultCode::NO_CMD_ARGS_AND_FAILED) {
            LOG4CPLUS_INFO(logger, "No command line args found and parsing failed...");
            LOG4CPLUS_INFO(logger, "Application will run with defaults...");
            commandLineInfoPtr = std::make_unique<SecretCommandLineInfo>();
        }

        if (cmdLineParserResultPair.second == ParserResultCode::CMD_ARGS_AND_FAILED) {
            LOG4CPLUS_INFO(logger, "Command line args found but parsing failed...");
            LOG4CPLUS_INFO(logger, "Application will run with defaults...");
            commandLineInfoPtr = std::make_unique<SecretCommandLineInfo>();
        }

        if (cmdLineParserResultPair.second == ParserResultCode::CMD_ARGS_AND_SUCCESS) {
            LOG4CPLUS_INFO(logger, "Command line args found and parsing succeeded...");
            LOG4CPLUS_INFO(logger, "Application will run with command line args...");
            commandLineInfoPtr = std::move(cmdLineParserResultPair.first);
        }

        if (cmdLineParserResultPair.second == ParserResultCode::NO_CMD_ARGS_AND_SUCCESS) {
            LOG4CPLUS_INFO(logger, "No command line args found and parsing succeeded...");
            LOG4CPLUS_INFO(logger, "Application will run with defaults...");
            commandLineInfoPtr = std::move(cmdLineParserResultPair.first);
        }

        LOG4CPLUS_INFO(logger, commandLineInfoPtr->m_sealKeys.ToString());
        return commandLineInfoPtr;
    }
} // yakbas
