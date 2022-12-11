#pragma once

#include "BaseClientManager.h"
#include "SharedCommunication.pb.h"
#include "SecretUser.h"

namespace yakbas::sec {

    class ClientManager : protected BaseClientManager {
    public:
        explicit ClientManager();

        ~ClientManager() override;

    private:
        const std::unique_ptr<SecretUser> userPtr{nullptr};
    };

} // yakbas
