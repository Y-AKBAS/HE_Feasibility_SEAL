#pragma once

#include <grpcpp/grpcpp.h>
#include "PublicCommunication.grpc.pb.h"

namespace yakbas::pub {

    class TransporterServiceImpl final : public communication::pub::PublicCommunicationService::Service {

    public:
        TransporterServiceImpl();

        grpc::Status StartUsing(grpc::ServerContext *context, const communication::StartUsingRequest *request,
                                communication::StartUsingResponse *response) override;

        grpc::Status EndUsing(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                              communication::EndUsingResponse *response) override;
    };

}// yakbas
