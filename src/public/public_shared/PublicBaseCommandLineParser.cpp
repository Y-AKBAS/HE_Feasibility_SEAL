
#include "PublicBaseCommandLineParser.h"

namespace yakbas::pub {

    PublicBaseCommandLineParser::~PublicBaseCommandLineParser() = default;

    std::pair<std::unique_ptr<PublicCommandLineInfo>, ParserResultCode>
    PublicBaseCommandLineParser::GetCommandLineInfo(const std::vector<std::string> &cmdLineArgs) {

        const constexpr char *portName = "portUrl,pu";
        const constexpr char *numberOfRequestsName = "numberOfRequests,nor";

        auto commandLineInfoPtr = std::make_unique<PublicCommandLineInfo>();

        po::options_description optionsDescription;
        optionsDescription.add_options()
                (numberOfRequestsName, po::value<int>(&commandLineInfoPtr->m_numberOfRequests))
                (portName, po::value<std::string>(&commandLineInfoPtr->m_portUrl));

        const bool success = IsParsed(cmdLineArgs, optionsDescription);

        PublicCommandLineInfo localCommandLineInfo{};
        const bool noArgs = *commandLineInfoPtr == localCommandLineInfo;

        ParserResultCode resultCode{};
        if (success && noArgs) {
            resultCode = ParserResultCode::NO_CMD_ARGS_AND_SUCCESS;
        }
        if (success && !noArgs) {
            resultCode = ParserResultCode::CMD_ARGS_AND_SUCCESS;
        }
        if (!success && noArgs) {
            resultCode = ParserResultCode::NO_CMD_ARGS_AND_FAILED;
        }
        if (!success && !noArgs) {
            resultCode = ParserResultCode::CMD_ARGS_AND_FAILED;
        }

        return std::make_pair<std::unique_ptr<PublicCommandLineInfo>, ParserResultCode>(
                std::move(commandLineInfoPtr),
                std::move(resultCode));
    }
} // yakbas