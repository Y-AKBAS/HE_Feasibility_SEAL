
#include "BaseClientManager.h"
#include <stdexcept>

namespace yakbas {

    BaseClientManager::BaseClientManager() = default;

    BaseClientManager::~BaseClientManager() = default;

    std::map<std::string, std::shared_ptr<grpc::Channel>> BaseClientManager::m_channelMap{};

    void BaseClientManager::AddChannel(const std::pair<std::string, std::shared_ptr<grpc::Channel>> &pair) {
        m_channelMap.insert(pair);
    }

    std::shared_ptr<grpc::Channel> &BaseClientManager::GetChannel(const std::string &channelName) {
        const auto it = m_channelMap.find(channelName);

        if (it != m_channelMap.end()) {
            return it->second;
        }

        throw std::invalid_argument("Channel: " + channelName + " doesn't exist!");
    }

} // yakbas