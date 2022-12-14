
#include <memory>
#include "SealInfo.h"
#include "SecretUser.h"

namespace yakbas::sec {

    class Generator {

    public:
        static std::unique_ptr<SecretUser> GenerateSecretUser(
                const SealKeys &sealKeys = {});

    };

} // yakbas
