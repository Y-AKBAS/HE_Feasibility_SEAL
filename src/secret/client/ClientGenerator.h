
#include <memory>
#include "SealInfo.h"
#include "SecretUser.h"
#include "SecretCommunication.pb.h"

namespace yakbas::sec {

    class ClientGenerator {

    public:
        static std::unique_ptr<SecretUser> GenerateSecretUser(
                const SealKeys &sealKeys = {});

        static std::unique_ptr<communication::sec::SearchRequest>
        GenerateSearchRequest(const std::unique_ptr<std::stringstream> &publicKeyBuffer);

    };

} // yakbas
