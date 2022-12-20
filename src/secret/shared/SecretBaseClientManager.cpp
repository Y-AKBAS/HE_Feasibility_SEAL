
#include "SecretBaseClientManager.h"

namespace yakbas::sec {

    using secretService = communication::sec::SecretCommunicationService;

    SecretBaseClientManager::SecretBaseClientManager() = default;

    SecretBaseClientManager::~SecretBaseClientManager() = default;

    std::unique_ptr<secretService::Stub> SecretBaseClientManager::GetStub(const std::string &channelName) const {
        return secretService::NewStub(GetChannel(channelName));
    }

} // yakbas
