
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

    void BaseClientManager::CreateChannels(const std::map<std::string, std::string> &namePortPair) {
        if (m_channelMap.empty()) {
            if (namePortPair.empty()) {
                throw std::invalid_argument("Name Port Pair cannot be empty.");
            }

            const static auto channelArgsPtr = []() -> decltype(auto) {
                auto argsPtr = std::make_unique<grpc::ChannelArguments>();
                constexpr int size = std::numeric_limits<int32_t>::max();
                argsPtr->SetMaxReceiveMessageSize(size);
                argsPtr->SetMaxSendMessageSize(size);
                return argsPtr;
            }();

            for (const auto &pair: namePortPair) {
                m_channelMap.insert(
                        std::make_pair(pair.first,
                                       grpc::CreateCustomChannel(pair.second,
                                                                 grpc::InsecureChannelCredentials(),
                                                                 *channelArgsPtr)));
            }
        }
    }

} // yakbas