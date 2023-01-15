#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/program_options.hpp>

namespace yakbas {

    enum class ParserResultCode : int {
        CMD_ARGS_AND_SUCCESS,
        CMD_ARGS_AND_FAILED,
        NO_CMD_ARGS_AND_SUCCESS,
        NO_CMD_ARGS_AND_FAILED,
    };

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
