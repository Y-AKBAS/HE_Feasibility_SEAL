#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>

namespace yakbas {

    class BaseClientManager {

    public:
        explicit BaseClientManager();

    protected:
        std::map<std::string, std::shared_ptr<grpc::Channel>> m_channelMap;
    public:
        decltype(auto) addChannel(const std::pair<std::string, std::shared_ptr<grpc::Channel>> &pair);

        decltype(auto) getChannel(const std::string &channelName);
    };

} // yakbas