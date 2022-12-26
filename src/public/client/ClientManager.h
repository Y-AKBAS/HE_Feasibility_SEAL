#pragma once

#include <log4cplus/logger.h>
#include "BaseClientManager.h"
#include "PublicUser.h"
#include "PublicCommunication.grpc.pb.h"
#include "PublicBaseClientManager.h"


namespace yakbas::pub {

    class ClientManager : public PublicBaseClientManager {
    public:

        ClientManager();

        [[nodiscard]] std::unique_ptr<std::vector<std::unique_ptr<communication::Journey>>>
        Search(const std::string &from, const std::string &to, int numberOfJourneys);

        static bool IsInitialized();

    private:
        const std::unique_ptr<PublicUser> m_userPtr{nullptr};
        std::unique_ptr<log4cplus::Logger> m_logger{nullptr};

        static std::once_flag m_isInitialized;
    };

} // yakbas
