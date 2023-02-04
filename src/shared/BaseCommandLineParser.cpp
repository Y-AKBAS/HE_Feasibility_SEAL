
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "BaseCommandLineParser.h"

namespace yakbas {

    const po::command_line_style::style_t BaseCommandLineParser::m_style = static_cast<const po::command_line_style::style_t>(
            (po::command_line_style::default_style ^ po::command_line_style::allow_guessing)
            | po::command_line_style::case_insensitive);

    bool BaseCommandLineParser::IsParsed(const std::vector<std::string> &cmdLineArgs,
                                         const po::options_description &optionsDescription,
                                         const po::positional_options_description &positionalOptionsDescription) {

        const log4cplus::Logger logger = log4cplus::Logger::getInstance("Parser Logger");

        try {
            po::variables_map vm;

            auto parser =
                    po::command_line_parser(cmdLineArgs)
                            .options(optionsDescription)
                            .style(m_style)
                            .allow_unregistered()
                            .positional(positionalOptionsDescription);

            po::store(parser.run(), vm);
            po::notify(vm);
            return true;
        }
        catch (const po::error &err) {
            LOG4CPLUS_ERROR(logger, "Failed to parse command-line args...");
            return false;
        }
    }

    bool BaseCommandLineParser::IsParsed(const std::vector<std::string> &cmdLineArgs,
                                         const po::options_description &optionsDescription) {

        const log4cplus::Logger logger = log4cplus::Logger::getInstance("Parser Logger");

        try {
            po::variables_map vm;

            auto parser =
                    po::command_line_parser(cmdLineArgs)
                            .options(optionsDescription)
                            .allow_unregistered()
                            .style(m_style);

            po::store(parser.run(), vm);
            po::notify(vm);
            return true;
        }
        catch (const po::error &err) {
            LOG4CPLUS_ERROR(logger, "Failed to parse command-line args...");
            return false;
        }
    }

    BaseCommandLineParser::~BaseCommandLineParser() = default;

    BaseCommandLineParser::BaseCommandLineParser() = default;

} // yakbas