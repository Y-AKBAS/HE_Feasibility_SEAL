
#include "Generator.h"
#include "Utils.h"

namespace yakbas::sec {
    using namespace yakbas::util;

    std::unique_ptr<SecretUser> Generator::GenerateSecretUser(const SealKeys &sealKeys) {

        auto addressPtr = std::make_unique<BaseUser::Address>(
                "My street",
                "My house number",
                "My postal code",
                "My city",
                "My country"
        );

        return std::make_unique<SecretUser>(
                GetUUID(),
                "Yasin",
                "Akbas",
                "my@gmail.com",
                addressPtr,
                std::make_unique<CustomSealOperations>(sealKeys)
        );
    }
} // yakbas