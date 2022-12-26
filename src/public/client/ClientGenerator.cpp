
#include "ClientGenerator.h"
#include "Utils.h"

namespace yakbas::pub {

    using namespace yakbas::util;

    std::unique_ptr<PublicUser> ClientGenerator::GeneratePublicUser() {


        auto addressPtr = std::make_unique<BaseUser::Address>(
                "My street",
                "My house number",
                "My postal code",
                "My city",
                "My country"
        );

        return std::make_unique<PublicUser>(
                GetUUID(),
                "Yasin",
                "Akbas",
                "my@gmail.com",
                addressPtr
        );
    }
} // yakbas
