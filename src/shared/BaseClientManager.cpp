
#include "BaseClientManager.h"

namespace yakbas {

    BaseClientManager::BaseClientManager() : m_channelMap{} {}

    decltype(auto) BaseClientManager::addChannel(const std::pair<std::string, std::shared_ptr<grpc::Channel>> &pair) {
        m_channelMap.insert(pair);
        return pair;
    }

    decltype(auto) BaseClientManager::getChannel(const std::string &channelName) {
        return m_channelMap.find(channelName);
    }

} // yakbas