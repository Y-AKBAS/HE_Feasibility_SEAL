#pragma once

#include <memory>
#include "PublicUser.h"

namespace yakbas::pub {

    class ClientGenerator {

    public:
        static std::unique_ptr<PublicUser> GeneratePublicUser();
    };

} // yakbas
