
#include "TransporterServiceImpl.h"

namespace yakbas::pub {
    TransporterServiceImpl::TransporterServiceImpl() = default;

    grpc::Status TransporterServiceImpl::StartUsing(grpc::ServerContext *context,
                                                    const communication::StartUsingRequest *request,
                                                    communication::StartUsingResponse *response) {
        return grpc::Status::OK;
    }

    grpc::Status
    TransporterServiceImpl::EndUsing(grpc::ServerContext *context, const communication::EndUsingRequest *request,
                                     communication::EndUsingResponse *response) {
        return grpc::Status::OK;
    }
} // yakbas