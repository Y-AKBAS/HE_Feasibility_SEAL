
#include "SecretBaseCommandLineParser.h"

namespace yakbas::sec {

    std::pair<std::unique_ptr<SecretCommandLineInfo>, ParserResultCode>
    SecretBaseCommandLineParser::GetCommandLineInfo(const std::vector<std::string> &cmdLineArgs) {

        const constexpr char *schemeTypeName = "schemeType,st";
        const constexpr char *plainModulusName = "plainModulus,pm";
        const constexpr char *polyModulusDegreeName = "polyModulusDegree,pmd";
        const constexpr char *isEncodingEnabledName = "encodingEnabled,ee";
        const constexpr char *scalePowerName = "scalePower,sp";
        const constexpr char *numberOfRequestsName = "numberOfRequests,nor";
        const constexpr char *portUrlName = "portUrl,pu";

        auto commandLineInfoPtr = std::make_unique<SecretCommandLineInfo>();
        auto &keys = commandLineInfoPtr->m_sealKeys;
        int schemeType{};

        po::options_description optionsDescription;
        optionsDescription.add_options()
                (schemeTypeName, po::value<int>(&schemeType))
                (isEncodingEnabledName, po::bool_switch(&keys.m_isEncodingEnabled))
                (plainModulusName, po::value<int>(&keys.m_plainModulus))
                (polyModulusDegreeName, po::value<std::size_t>(&keys.m_polyModulusDegree))
                (scalePowerName, po::value<int>(&keys.m_scalePower))
                (numberOfRequestsName, po::value<int>(&commandLineInfoPtr->m_numberOfRequests))
                (portUrlName, po::value<std::string>(&commandLineInfoPtr->m_portUrl));

        const bool success = IsParsed(cmdLineArgs, optionsDescription);

        if (schemeType > 0) {
            keys.m_schemeType = static_cast<seal::scheme_type>(schemeType);
        }

        SecretCommandLineInfo localCommandLineInfo{};
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

        return std::make_pair<std::unique_ptr<SecretCommandLineInfo>, ParserResultCode>(
                std::move(commandLineInfoPtr),
                std::move(resultCode));
    }

    SecretBaseCommandLineParser::SecretBaseCommandLineParser() = default;

    SecretBaseCommandLineParser::~SecretBaseCommandLineParser() = default;

} // yakbas
