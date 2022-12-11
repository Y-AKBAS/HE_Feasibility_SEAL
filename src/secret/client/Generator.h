
#include <memory>
#include "SealInfo.h"

namespace yakbas::sec {

    class Generator {

    public:
        static decltype(auto) GenerateSecretUser(const SealKeys &sealKeys = {seal::scheme_type::bfv, 16384, 1024});

    };

} // yakbas
