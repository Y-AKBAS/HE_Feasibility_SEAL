
#pragma once

#include <vector>
#include <memory>
#include "SecretCommunication.grpc.pb.h"


namespace yakbas::sec {
    class PlatformGenerator {
    public:
        static std::unique_ptr<std::vector<communication::sec::Ride>> GenerateRides();

    };

} // yakbas
