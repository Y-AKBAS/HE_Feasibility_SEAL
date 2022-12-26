#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>
#include <utility>

namespace yakbas {

    class BaseClientManager {

    public:
        explicit BaseClientManager();

        virtual ~BaseClientManager();

    protected:
        static std::map<std::string, std::shared_ptr<grpc::Channel>> m_channelMap;

        static void CreateChannels(const std::map<std::string, std::string> &namePortPair);

    public:
        static void AddChannel(const std::pair<std::string, std::shared_ptr<grpc::Channel>> &pair);

        static std::shared_ptr<grpc::Channel> &GetChannel(const std::string &channelName);
    };

} // yakbas