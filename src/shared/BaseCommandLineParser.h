#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

#include <boost/program_options.hpp>

namespace yakbas {

    enum class ParserResultCode : int {
        CMD_ARGS_AND_SUCCESS,
        CMD_ARGS_AND_FAILED,
        NO_CMD_ARGS_AND_SUCCESS,
        NO_CMD_ARGS_AND_FAILED,
    };

    template<class T>
    std::unique_ptr<T> PrintCmdLineStateAndExtractPtr(
            const log4cplus::Logger &logger,
            std::pair<std::unique_ptr<T>, ParserResultCode> &cmdLineParserResultPair
    ) {
        std::unique_ptr<T> commandLineInfoPtr{nullptr};

        if (cmdLineParserResultPair.second == ParserResultCode::NO_CMD_ARGS_AND_FAILED) {
            LOG4CPLUS_INFO(logger, "No command line args found and parsing failed...");
            LOG4CPLUS_INFO(logger, "Application will run with defaults...");
            commandLineInfoPtr = std::make_unique<T>();
        }

        if (cmdLineParserResultPair.second == ParserResultCode::CMD_ARGS_AND_FAILED) {
            LOG4CPLUS_INFO(logger, "Command line args found but parsing failed...");
            LOG4CPLUS_INFO(logger, "Application will run with defaults...");
            commandLineInfoPtr = std::make_unique<T>();
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

        return std::move(commandLineInfoPtr);
    }

    namespace po = boost::program_options;

    class BaseCommandLineParser {

    public:

        BaseCommandLineParser();

        virtual ~BaseCommandLineParser();

        static bool IsParsed(const std::vector<std::string> &cmdLineArgs,
                             const po::options_description &optionsDescription,
                             const po::positional_options_description &positionalOptionsDescription);

        static bool IsParsed(const std::vector<std::string> &cmdLineArgs,
                             const po::options_description &optionsDescription);

        static const po::command_line_style::style_t m_style;
    };

}
