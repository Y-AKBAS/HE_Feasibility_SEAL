#pragma once

#include "BaseCommandLineParser.h"
#include "SecretCommandLineInfo.h"

namespace yakbas::sec {

    class SecretBaseCommandLineParser : public BaseCommandLineParser {

    public:
        static std::pair<std::unique_ptr<SecretCommandLineInfo>, ParserResultCode>
        GetCommandLineInfo(const std::vector<std::string> &cmdLineArgs);

        SecretBaseCommandLineParser();

        ~SecretBaseCommandLineParser() override;
    };

} // yakbas

