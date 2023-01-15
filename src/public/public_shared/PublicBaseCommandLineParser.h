#pragma once

#include "BaseCommandLineParser.h"
#include "PublicCommandLineInfo.h"

namespace yakbas::pub {

    class PublicBaseCommandLineParser : public BaseCommandLineParser {

    public:
        static std::pair<std::unique_ptr<PublicCommandLineInfo>, ParserResultCode>
        GetCommandLineInfo(const std::vector<std::string> &cmdLineArgs);

        ~PublicBaseCommandLineParser() override;
    };

} // yakbas
