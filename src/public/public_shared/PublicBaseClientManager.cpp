
#include "PublicBaseClientManager.h"

namespace yakbas::pub {

    using publicService = communication::pub::PublicCommunicationService;

    PublicBaseClientManager::PublicBaseClientManager() = default;

    PublicBaseClientManager::~PublicBaseClientManager() = default;

    std::unique_ptr<publicService::Stub> PublicBaseClientManager::GetStub(const std::string &channelName) const {
        return publicService::NewStub(GetChannel(channelName));
    }


} // yakbas
